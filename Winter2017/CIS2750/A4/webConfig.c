#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

//function pointers
void createFiles(char *);
void writeToHTMLfile(char * token, FILE  *);
char * splitByPeriod(char *, int *);
int hasHeaderTags(char * string);
int hasButtonTags(char * string);
int hasExeTags(char * string);
char * getValueInQuotes(char * string);
int hasInputTags(char * string); 
int hasLinkTags(char * string);
int hasPictureTags(char * string);
int hasRadioTags(char * string);
int hasTextTags(char * string);

//for testing
int main (int argc, char  * argv[]) {
    createFiles(argv[1]);
}

void createFiles(char * name_of_file) {
    FILE * config_file = fopen(name_of_file, "r");

    if(config_file == NULL) {
        puts("Error reading file.");
        return;
    }
    
    char * line = NULL;
    char * file_value = NULL;
    char * old_file_value = NULL;
    size_t len = 10;
    int error = 0;

    /* get the file strings and store them within a string */
    while (1) {
        error = getline(&line, &len, config_file);
        
        if(error == -1) break;
        line[strlen(line) - 1] = '\0';

        old_file_value = file_value;
        
        if (old_file_value != NULL) {
            file_value = (char*)malloc(strlen(old_file_value) + strlen(line) + 1);
            old_file_value[strlen(old_file_value) - 1] = '\0';
            sprintf(file_value, "%s%s", old_file_value, line);
            free(old_file_value);   
        } else {
            file_value = strdup(line);
        }
    }

    int split_pos = 0;

    char * token = splitByPeriod(file_value, &split_pos);
    char * remaining_value = &file_value[split_pos];
    
    while(split_pos != -1) {
        writeToHTMLfile(token, stdout);
        free(token);

        token = splitByPeriod(remaining_value, &split_pos);
        remaining_value = &remaining_value[split_pos];
    }

    //get the last one
    writeToHTMLfile(token, stdout);
    free(token);
    
    free(file_value);
    free(line);
}

char * splitByPeriod(char * the_string, int * split_pos) {
    char * return_value = strdup(the_string);
    if (*the_string == '.') {
        the_string++; // the first value is a period
    }
    
    int in_quotes = 0;
    int counter = 0;

    while(*the_string != '\0') {
        if(*the_string == '"') {
            in_quotes += 1;
        } else if (*the_string == '.') {
            if((in_quotes % 2) == 0) { // we arent in quotes right now 
                return_value[counter + 1] = '\0';
                *split_pos = counter + 1;
                return return_value;
            }
        }

        the_string ++;
        counter ++;
    }

    /* we are now at the end of the token without find a period inbetween spaces
        so we are already at the end */
    *split_pos = -1;
    return return_value;
}

void writeToHTMLfile(char * value_of_token, FILE * output_file) {
    char * token = strdup(value_of_token);
    char * remaining_value;
    char * value_in_quotes;
    if (*token == '.') {
        token += 1; //first in the period
    }
    token[strlen(token)] = '\0';
    char type = token[0];
    int tags = 0;
    int amount_removed = 0;

    if (type == 'h') {
        char * remaining_text = NULL;
        char * text = strdup("HEADING");
        int size = 3;
        while(token != NULL) {
            tags = hasHeaderTags(token);
            if(tags == 1) { // text value, get the value in quotes
                remaining_value = strtok(token, "size=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                char * temp = value_in_quotes;
                size = strtol(temp, &temp, 10);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if (tags == 2) { // text value, get the value in quotes
                remaining_value = strtok(token, "text=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(text);
                text = strdup(value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else { // there are no more tags left, just print out the rest to the html
                if(*token != '\0' && *token != ')') {
                    if(token[0] == 'h' && token[1] == '(') {
                        token += 2;
                    }
                    if(token[0] == ',') {
                        token += 1;
                    }
                    remaining_text = strdup(token);
                }
                break;
            }
            token = remaining_value + amount_removed + 1;
        }
        
        //print to file
        if(remaining_text == NULL) {
            fprintf(output_file, "<h%d>%s", size, text);
        } else {
            fprintf(output_file, "<h%d>%s %s", size, text, remaining_text);
            free(remaining_text);
        }
        fprintf(output_file, "</h%d>\n", size);

        free(text);  
    } else if (type == 'b') {
        char * remaining_text = NULL;
        char * name = strdup("button");
        char * link = strdup("link");
        char * display = strdup("inline");
        while(token != NULL) {
            tags = hasButtonTags(token);
            if(tags == 1) { // text value, get the value in quotes
                remaining_value = strtok(token, "name=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(name);
                name = strdup(value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if (tags == 2) { // text value, get the value in quotes
                remaining_value = strtok(token, "link=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(link);
                link = strdup(value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if (tags == 3) { // text value, get the value in quotes
                remaining_value = strtok(token, "display=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 7;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(display);
                display = strdup(value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            }else { // there are no more tags left, just print out the rest to the html
                if(*token != '\0' && *token != ')') {
                    if(token[0] == 'b' && token[1] == '(') {
                        token += 2;
                    }
                    if(token[0] == ',') {
                        token += 1;
                    }
                    remaining_text = strdup(token);
                    if(remaining_text[strlen(remaining_text) - 2] == ')') remaining_text[strlen(remaining_text) - 2] = '\0';
                }
                break;
            }
            token = remaining_value + amount_removed + 1;
        }

        //print to file
        fprintf(output_file, "<form method=\"POST\" action=\"%s\" style=\"display:%s\";>\n"
            "<input type=\"submit\" value=\"%s\"/>\n", link, display, name);
        fprintf(output_file, "<input type=\"hidden\" name=\"Sender\" value=\"%s\">", name);
        if(remaining_text != NULL) {
            fprintf(output_file, "%s\n", remaining_text);
            free(remaining_text);
        }
        fprintf(output_file, "</form>\n");

        free(display);
        free(name);
        free(link);
    } else if (type == 'd') {
        fprintf(output_file, "<hr>\n");
        //get rid of the d()
        token += 2;
        token[strlen(token) - 2] = '\0'; 
        fprintf(output_file, "%s\n", token);
    } else if(type == 'a') {
        fprintf(output_file, "<form method=\"POST\", action=\"addauthor.php\">\n");
        fprintf(output_file, "Enter Author:<br><input type=\"text\" name=\"addauthor\" value=\"Type Here...\"><br><br>Enter Stream:<br><input type=\"text\" name=\"addstream\" value=\"Enter Stream...\"><br><br>Add/Remove:<br><input type=\"text\" name=\"addremove\" value=\"Enter either add or remove\"><br><br><input type=\"submit\" value=\"Submit\"><input type=\"hidden\" name=\"Sender\" value=\"Submit\"></form>");
    } else if(type == 'z') {
        fprintf(output_file, "<form method=\"POST\", action=\"post.php\">\n");
        fprintf(output_file, "Enter Post:<br><input type=\"text\" name=\"post\" value=\"Type Here...\"><br><br><input type=\"submit\" value=\"Submit\"><input type=\"hidden\" name=\"Sender\" value=\"Submit\"></form>");
    } else if (type == 'e') {
        char * remaining_text = NULL;
        char * exe = strdup("standard");
        while(token != NULL) {
            tags = hasExeTags(token);
            if(tags == 1) { // text value, get the value in quotes
                remaining_value = strtok(token, "exe=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 3;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(exe);
                exe = strdup(value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else { // there are no more tags left, just print out the rest to the html
                if(*token != '\0' && *token != ')') {
                    if(token[0] == 'h' && token[1] == '(') {
                        token += 2;
                    }
                    if(token[0] == ',') {
                        token += 1;
                    }
                    remaining_text = strdup(token);
                    if(remaining_text[strlen(remaining_text) - 2] == ')') remaining_text[strlen(remaining_text) - 2] = '\0';

                }
                break;
            }
            token = remaining_value + amount_removed + 1;
        } 

        int printed_php = 1;

        //print to file
        if (access(exe, F_OK) != -1) {
            fprintf(output_file, "<?php echo htmlspecialchars(\"exec(\'%s\'\")", exe);
        } else {
            char * new_exec = malloc(sizeof(exe) + sizeof("bin/") + 1);
            sprintf(new_exec, "bin/%s", exe);
            if (access(new_exec, F_OK) != -1) {
                fprintf(output_file, "<?php echo htmlspecialchars(\"exec(\'%s\'\")", new_exec);
            } else {
                printed_php = 0;
                fprintf(output_file, "EXE not found in current directory or in bin. <br>");
            }
        }
        
        if(remaining_text != NULL) {
            fprintf(output_file, "%s\n", remaining_text);
            free(remaining_text);
        }

        if (printed_php) fprintf(output_file, "?>\n");

        free(exe);
    } else if (type == 'i') {
        //The inputs need to be in order or the world will end
        char * remaining_text = NULL;
        char * name = strdup("default");

        fprintf(output_file, "<form ");
        while(token != NULL) {
            tags = hasInputTags(token);
            if(tags == 1) { // text value, get the value in quotes
                remaining_value = strtok(token, "action=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 6;
                value_in_quotes = getValueInQuotes(remaining_value);
                
                fprintf(output_file, "method=\"POST\", action=\"%s\">\n", value_in_quotes);

                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if(tags == 2) { // text value, get the value in quotes
                remaining_value = strtok(token, "text=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                
                fprintf(output_file, "%s:<br>\n", value_in_quotes);

                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if(tags == 3) { // text value, get the value in quotes
                remaining_value = strtok(token, "name=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(name);
                name = strdup(value_in_quotes);

                fprintf(output_file, "<input type=\"text\" name=\"%s\" ", value_in_quotes);

                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if(tags == 4) { // text value, get the value in quotes
                remaining_value = strtok(token, "value=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 5;
                value_in_quotes = getValueInQuotes(remaining_value);

                fprintf(output_file, "value=\"%s\"><br><br>\n", value_in_quotes);
                

                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else { // there are no more tags left, just print out the rest to the html
                fprintf(output_file, "<input type=\"submit\" value=\"Submit\">\n");
                fprintf(output_file, "<input type=\"hidden\" name=\"Sender\" value=\"Submit\">");
                if(*token != '\0' && *token != ')') {
                    if(token[0] == 'i' && token[1] == '(') {
                        token += 2;
                    }
                    if(token[0] == ',') {
                        token += 1;
                    }
                    remaining_text = strdup(token);
                    if(remaining_text[strlen(remaining_text) - 2] == ')') remaining_text[strlen(remaining_text) - 2] = '\0';
                }
                break;
            }
            token = remaining_value + amount_removed + 1;
        } 

        if(remaining_text != NULL) {
            fprintf(output_file, "%s\n", remaining_text);
            free(remaining_text);
        }
    
        fprintf(output_file, "</form>\n");
        free(name);
    } else if (type == 'l') {
        char * remaining_text = NULL;
        char * text = strdup("link");
        char * link = strdup("google.com");
        while(token != NULL) {
            tags = hasLinkTags(token);
            if(tags == 1) { // text value, get the value in quotes
                remaining_value = strtok(token, "text=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(text);
                text = strdup(value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if(tags == 2) { // text value, get the value in quotes
                remaining_value = strtok(token, "link=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(link);
                link = strdup(value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else { // there are no more tags left, just print out the rest to the html
                if(*token != '\0' && *token != ')') {
                    if(token[0] == 'h' && token[1] == '(') {
                        token += 2;
                    }
                    if(token[0] == ',') {
                        token += 1;
                    }
                    remaining_text = strdup(token);
                    remaining_text[strlen(remaining_text) - 2] = '\0';
                }
                break;
            }
            token = remaining_value + amount_removed + 1;
        }

        fprintf(output_file, "<a href=\"%s\">%s ", link, text);
        if(remaining_text != NULL) {
            fprintf(output_file, "%s", remaining_text);
            free(remaining_text);
        }

        free(link);
        free(text);
    } else if (type == 'p') {
        char * remaining_text = NULL;
        char * name = strdup("default");
        int w,h = 100;
        while(token != NULL) {
            tags = hasPictureTags(token);
            if(tags == 1) { // text value, get the value in quotes
                remaining_value = strtok(token, "image=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 5;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(name);
                name = strdup(value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if(tags == 2) { // text value, get the value in quotes
                remaining_value = strtok(token, "size=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                amount_removed = strlen(value_in_quotes) + 1;
                
                char * temp = strtok(value_in_quotes, "x");
                w = (int)strtol(temp, &temp, 10);
                temp = strtok(NULL, "");
                h = (int)strtol(temp, &temp, 10);

                free(value_in_quotes);
            } else { // there are no more tags left, just print out the rest to the html
                if(*token != '\0' && *token != ')') {
                    if(token[0] == 'h' && token[1] == '(') {
                        token += 2;
                    }
                    if(token[0] == ',') {
                        token += 1;
                    }
                    remaining_text = strdup(token);
                    remaining_text[strlen(remaining_text) - 2] = '\0';
                }
                break;
            }
            token = remaining_value + amount_removed + 1;
        }  

        fprintf(output_file, "<img src=\"%s\" alt=\"%s\" style=\"width:%dpx;height:%dpx;\" ", name, name, w, h);
        if(remaining_text != NULL) {
            fprintf(output_file, "%s", remaining_text);
            free(remaining_text);
        }
        fprintf(output_file, ">\n");


        free(name);
    } else if (type == 'r') {
        //Need correct ordering of tags
        char * remaining_text = NULL;
        int first_one = 1;

        char * name = strdup("default");
        fprintf(output_file, "<form method=\"post\"");
        while(token != NULL) {
            tags = hasRadioTags(token);
            if(tags == 1) { // text value, get the value in quotes
                remaining_value = strtok(token, "action=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 6;
                value_in_quotes = getValueInQuotes(remaining_value);
                fprintf(output_file, "action=\"%s\">\n", value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if(tags == 2) { // text value, get the value in quotes
                remaining_value = strtok(token, "name=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                free(name);
                name = strdup(value_in_quotes);
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else if(tags == 3) { // text value, get the value in quotes
                remaining_value = strtok(token, "value=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 5;
                value_in_quotes = getValueInQuotes(remaining_value);
                if(first_one == 1) {
                    fprintf(output_file, "<input type=\"radio\" name=\"%s\" value=\"%s\" checked> %s ", name, value_in_quotes, value_in_quotes);
                    first_one = 0;
                } else {
                    fprintf(output_file, "<br>\n<input type=\"radio\" name=\"%s\" value=\"%s\"> %s", name, value_in_quotes, value_in_quotes);
                }
                
                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
            } else { // there are no more tags left, just print out the rest to the html
                if(*token != '\0' && *token != ')') {
                    if(token[0] == 'h' && token[1] == '(') {
                        token += 2;
                    }
                    if(token[0] == ',') {
                        token += 1;
                    }
                    remaining_text = strdup(token);
                    remaining_text[strlen(remaining_text) - 2] = '\0';
                }
                break;
            }
            token = remaining_value + amount_removed + 1;
        }

        if(remaining_text != NULL) {
            fprintf(output_file, "\n%s", remaining_text);
            free(remaining_text);
        }
    
        fprintf(output_file, "<input type=\"submit\" name=\"submit\" value=\"submit\"/>");
        fprintf(output_file, "\n</form>\n");  
    } else if (type == 't') {
        int end = 0;
        char * remaining_text = NULL;
        fprintf(output_file, "<p>");
        while(token != NULL) {
            tags = hasTextTags(token);
            if(tags == 1) { // text value, get the value in quotes
                remaining_value = strtok(token, "file=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                
                FILE * fp = fopen(value_in_quotes, "r");
                char text[256];

                if(fp != NULL) {
                    while(fgets(text, 256, fp) != NULL) {
                        fprintf(output_file, "%s<br>", text);
                    }
                    fclose(fp);
                }

                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
                end = 1;
            } else if(tags == 2) { // text value, get the value in quotes
                remaining_value = strtok(token, "text=");
                remaining_value = strtok(NULL, "");
                remaining_value = remaining_value + 4;
                value_in_quotes = getValueInQuotes(remaining_value);
                
                fprintf(output_file, "%s", value_in_quotes);

                amount_removed = strlen(value_in_quotes) + 1;
                free(value_in_quotes);
                end = 1;
            } else if(end == 1) { // there are no more tags left, just print out the rest to the html
                if(*token != '\0' && *token != ')') {
                    if(token[0] == 'h' && token[1] == '(') {
                        token += 2;
                    }
                    if(token[0] == ',') {
                        token += 1;
                    }
                    remaining_text = strdup(token);
                    if(remaining_text[strlen(remaining_text) - 2] == ')') remaining_text[strlen(remaining_text) - 2] = '\0';
                    fprintf(output_file, "%s</p>", remaining_text);
                }
                break;
            }
            token = remaining_value + amount_removed + 1;
        } 
    }
    // free(token);
}

char * getValueInQuotes(char * string) {
    char * value = string + 1;
    char * temp = strdup(value);
    int len = strlen(temp);
    int i;
    
    for(i = 0;i < len;i++) {
        if(temp[i] == '"') {
            temp[i] = '\0';
        }
    }
    return temp;
}


int hasHeaderTags(char * string) {
    char * value_of_string = strdup(string);
    char * token = strtok(value_of_string, ",");

    while(token != NULL) {
        if (strstr(token, "size=") != NULL) {
            return 1;
        } else if (strstr(token, "text=") != NULL) {
            return 2;
        } else {
            return 0;
        }
        token = strtok(NULL, ",");
    }
    free(value_of_string);
}

int hasButtonTags(char * string) {
    char * value_of_string = strdup(string);
    char * token = strtok(value_of_string, ",");

    while(token != NULL) {
        if (strstr(token, "name=") != NULL) {
            return 1;
        } else if (strstr(token, "link=") != NULL) {
            return 2;
        } else if (strstr(token, "display=") != NULL) {
            return 3;
        } else {
            return 0;
        }
        token = strtok(NULL, ",");
    }
    free(value_of_string);
}

int hasExeTags(char * string) {
    char * value_of_string = strdup(string);
    char * token = strtok(value_of_string, ",");

    while(token != NULL) {
        if (strstr(token, "exe=") != NULL) {
            return 1;
        } else {
            return 0;
        }
        token = strtok(NULL, ",");
    }
    free(value_of_string);
}

int hasInputTags(char * string) {
    char * value_of_string = strdup(string);
    char * token = strtok(value_of_string, ",");

    while(token != NULL) {
        if (strstr(token, "action=") != NULL) {
            return 1;
        } else if (strstr(token, "text=") != NULL) {
            return 2;
        } else if (strstr(token, "name=") != NULL) {
            return 3;
        } else if (strstr(token, "value=") != NULL) {
            return 4;
        }  else {
            return 0;
        }
        token = strtok(NULL, ",");
    }
    free(value_of_string);
}

int hasLinkTags(char * string) {
    char * value_of_string = strdup(string);
    char * token = strtok(value_of_string, ",");

    while(token != NULL) {
        if (strstr(token, "text=") != NULL) {
            return 1;
        } else if (strstr(token, "link=") != NULL) {
            return 2;
        } else {
            return 0;
        }
        token = strtok(NULL, ",");
    }
    free(value_of_string);
}

int hasPictureTags(char * string) {
    char * value_of_string = strdup(string);
    char * token = strtok(value_of_string, ",");

    while(token != NULL) {
        if (strstr(token, "image=") != NULL) {
            return 1;
        } else if (strstr(token, "size=") != NULL) {
            return 2;
        } else {
            return 0;
        }
        token = strtok(NULL, ",");
    }
    free(value_of_string);
}

int hasRadioTags(char * string) {
    char * value_of_string = strdup(string);
    char * token = strtok(value_of_string, ",");

    while(token != NULL) {
        if (strstr(token, "action=") != NULL) {
            return 1;
        } else if (strstr(token, "name=") != NULL) {
            return 2;
        } else if (strstr(token, "value=") != NULL) {
            return 3;
        } else {
            return 0;
        }
        token = strtok(NULL, ",");
    }
    free(value_of_string);
}

int hasTextTags(char * string) {
    char * value_of_string = strdup(string);
    char * token = strtok(value_of_string, ",");

    while(token != NULL) {
        if (strstr(token, "file=") != NULL) {
            return 1;
        } else if (strstr(token, "text=") != NULL) {
            return 2;
        } else {
            return 0;
        }
        token = strtok(NULL, ",");
    }
    free(value_of_string);
}