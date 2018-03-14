with Ada.Text_IO; use Ada.Text_IO;
with ada.strings.unbounded.Text_IO; use ada.strings.unbounded.Text_IO;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
with Ada.Strings.Fixed;
with Ada.Strings.Maps.Constants; use Ada.Strings.Maps.Constants;
with Ada.Containers; use Ada.Containers;
with Ada.Containers.Generic_Array_Sort;
with Ada.Strings.Hash;
with Ada.Containers.Hashed_Maps; use Ada.Containers;
with Ada.Containers.Doubly_Linked_Lists; use Ada.Containers;
with ada.Text_IO; use Ada.Text_IO;

procedure anagram is

    -- Hash Map functions --------------------------------------------------------------------
    -- For Hash Map
    function Equivalent_Key (Left, Right : Unbounded_String) return Boolean is
    begin
        return Left = Right;
    end Equivalent_Key;
 
    function Hash_Func(Key : Unbounded_String) return Ada.Containers.Hash_Type is
    begin
        return Ada.Strings.Hash(To_String(Key));
    end Hash_Func;

    -- Hash Map values are linked lists
    package my_list is new Doubly_Linked_Lists(Unbounded_String); use my_list;

    package My_Hash is new Ada.Containers.Hashed_Maps(
	Key_Type => Unbounded_String,Element_Type => list,
			Hash => Hash_Func,Equivalent_Keys => Equivalent_Key);
  
    -- main map that will contain sorted strings that represent english words and than the values
    -- will be a list of the words that have the same sorted string value (aka all the anagrams of each other)
    anagram_map : My_Hash.Map;

    -- Simplifying the inserting into map proccess because if we want to insert a value that isnt in the map yet, 
    -- we need to create a new list first, and than insert the new list, whereas is the list already exists, we want to
    -- append to that list, and than replace the value in the hash map
    procedure InsertIntoMap(temp_map : in out My_Hash.Map; temp_key, temp_value : in Unbounded_String) is 
        temp_list : list;
    begin
        if(temp_map.contains(temp_key)) then
            temp_list := temp_map(temp_key);
            temp_list.append(temp_value);
            temp_map.replace(key => temp_key, new_item => temp_list);
        else 
            temp_list.append(temp_value);
            temp_map.insert(key => temp_key, new_item => temp_list);
        end if;
    end InsertIntoMap;

    -- makes printing the map easy (with just map.Iterate (Print_Node'Access);)
    procedure Print_Node (Position : Cursor) is
    begin
        put_line(to_string(Element (Position)));
    end Print_Node;
    
    -- End of Hash Map functions --------------------------------------------------------------------

    -- used to sort the strings
    procedure String_Sort is new Ada.Containers.Generic_Array_Sort (Positive, Character, String);

    -- Required Subprograms -----------------------------------------------------------------

    procedure buildLEXICON (map : out My_Hash.map) is
        infp : file_type;
        word : Unbounded_String;
    begin
        open(infp, in_file, "words");
        loop
            exit when end_of_file(infp);
            get_line(infp,word);
            declare
                word_string : string := to_string(word);
            begin
                -- sort the word we read in, then save that to the map with the sorted word as the key, and the 
                -- unsorted word as an addition to the linked list
                -- sorted words are first converted to lower case
                String_Sort(word_string);
                Ada.Strings.Fixed.Translate(word_string, Ada.Strings.Maps.Constants.Lower_Case_Map);
                InsertIntoMap(map, to_unbounded_string(word_string), word);
            end;
        end loop;
        close(infp);
    end buildLEXICON;

    procedure findAnagram(word : String; anagram_map : My_Hash.map) is
        temp_list : list;
        procedure print_word (Position : Cursor) is
        begin
            put(to_string(Element (Position)) & " ");
        end print_word;
    begin
        if(anagram_map.contains(to_unbounded_string(word))) then
            temp_list := anagram_map(to_unbounded_string(word)); 
            put(word & "    ");
            temp_list.Iterate(print_word'Access);
            new_line;
        else 
            put_line(word & "   Did not find any anagrams");
        end if;
    end findAnagram;

    procedure generateAnagram(jumbled_words : list; anagram_map : My_Hash.map) is
        procedure sort (Position : Cursor) is
        begin
            declare
                temp_string : string := to_string(Element(Position));
            begin
                Ada.Strings.Fixed.Translate(temp_string, Ada.Strings.Maps.Constants.Lower_Case_Map);    
                String_Sort(temp_string);
                findAnagram(temp_string, anagram_map);
            end;
        end sort;
    begin
        jumbled_words.Iterate (sort'Access);
    end generateAnagram;

    procedure inputJumble (anagram_map : My_Hash.map) is
        words : list;
        word : Unbounded_String;
        input : Unbounded_String;
        finished_input : integer := 0;
        end_program : integer := 0;
    begin
    while (end_program /= 1) loop
        finished_input := 0;
        words.clear;
        put_line("Enter the words (up to 50 chars per word) that you want to find anagrams of: (Enter a # sign when you want to continue)");
        while (finished_input /= 1) loop
            Get_Line(word);

            if (to_string(word)(1) = '#') then
                finished_input := 1;
            else
                words.append(word);
            end if;
        end loop;

        generateAnagram(words, anagram_map);

        put_line("Do you want to find more anagrams? (Y or N)");
        Get_Line(input);

        if(to_string(input)(1) = 'N') then
            end_program := 1;
        end if;
    end loop;
    end inputJumble;

    -- end of Required Subprograms -----------------------------------------------------------------
    
begin
    buildLEXICON(anagram_map);
    inputJumble(anagram_map);
    
end anagram;