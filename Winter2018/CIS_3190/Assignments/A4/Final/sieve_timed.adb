with Ada.Text_IO; use Ada.Text_IO;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
with Ada.Text_IO.Unbounded_IO;
with Ada.Calendar; use Ada.Calendar;

procedure sieve_timed is
    upper_lim_string : String (1..7);
    int_array : array (0..1000000) of Integer;
    prime_array : array(0..1000000) of Integer;
    len : Natural;
    upper_lim : Integer;
    i, j, prime_counter : Integer;
    F : Ada.Text_IO.File_Type;
    output : Unbounded_String;

    type Proc_Access is access procedure(X : in out Integer);
    function Time_It(Action : Proc_Access; Arg : Integer) return Duration is
        Start_Time : Time := Clock;
        Finis_Time : Time;
        Func_Arg : Integer := Arg;
    begin
        Action(Func_Arg);
        Finis_Time := Clock;
        return Finis_Time - Start_Time;
    end Time_It;

    procedure sieve_run(X : in out Integer) is
    begin
        i := 0;
        loop
            exit when i >= upper_lim;
            int_array(i) := i;
            i := i + 1;
        end loop;

        i := 2;
        loop    
            exit when (i*i) >= upper_lim;
            if(int_array(i) /= -1) then
                j := i*2;
                loop
                    exit when j >= upper_lim;
                    int_array(j) := -1;
                    j := j + i;
                end loop;
            end if;
            i := i + 1;
        end loop;

        prime_counter := 0;

        i := 2;
        loop    
            exit when i >= upper_lim;
            if(int_array(i) /= -1) then
                prime_array(prime_counter) := int_array(i);
                prime_counter := prime_counter + 1;
            end if;
            i := i + 1;
        end loop;

        put_line(Integer'Image(prime_counter));

        Create(F, Ada.Text_IO.Out_File, "output.txt");

        i := 0;
        loop
            exit when i >= prime_counter;
            output := to_unbounded_string(Integer'Image(prime_array(i)));
            Unbounded_IO.put_line(F, output);
            i := i + 1;
        end loop;
        
        Close(F);
    end sieve_run;

    sieve_access : Proc_Access := sieve_run'access;
begin
    loop 
        put_line("Enter a upper limit from 2-1000000");
        get_line(upper_lim_string, len);

        upper_lim := Integer'Value(upper_lim_string ( upper_lim_string'First..upper_lim_string'First + len -1 ));

        if(upper_lim <= 2 or upper_lim > 1000000) then
            put_line("Enter a upper limit from 2-1000000");
        else
            exit;
        end if;
    end loop;

    Put_Line("Sieve runtime is " & Duration'Image(Time_It(sieve_access, 4)) & " seconds");

end sieve_timed;