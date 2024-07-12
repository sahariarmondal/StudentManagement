/*Write a program to store student information in a file and do the following operations.Information includes roll,name and score in five subjects. Useer*/
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    
    // Creating Structure for Storing the Information of Students.
    typedef struct student
    {
        char name[25];
        int roll;
        int marks[5];
        int totalscore;
        bool isValid;
    
    } student;
    
    // Function to add Students in the file.
    void addStudent(FILE *fp);
    
    // function to search a name in the file by providing the roll
    void search(FILE *fp);
    
    // function to modify the info of a student
    void modifyInfo(FILE *fp);
    
    /*function to delete a student information from the list.
      both logical and physical deletation will be done by this function.
      Physical deletaion will be done when number of logically deleted
      student information will be half of the total number of elements of
      the file then all the logically deleted elements will be deleted
      physically from the file*/
    void delete(FILE *fp);
    
    // function to display all the valid students information
    void display(FILE *fp);
    void addStudent(FILE *fp)
    {
    
        int roll, i = 0, tscore = 0, tempmarks = 0;
    
        // Allocating memory for storing the information of the students
        student *temp = (student *)malloc(sizeof(student));
    
        rewind(fp);
    
        printf("Enter Roll No: ");
        scanf(" %d", &roll);
    
        // loop for checking wheather the roll no already exist or not.
        while (fread(temp, sizeof(student), 1, fp))
        {
        
            if (temp->roll == roll && temp->isValid)
            {
            
                printf("The roll no is already Exist\n");
                free(temp);
                return;
            }
        }
    
        temp->roll = roll;
        printf("Enter the name of the student:\n");
        scanf(" %s", temp->name);
    
        // Getting the marks of the students.
    
        for (i = 0; i < 5; i++)
        {
        
            printf("Enter the marks of sub-%d\n", i + 1);
            scanf(" %d", &tempmarks);
    
            tscore += tempmarks;
            temp->marks[i] = tempmarks;
        }
    
        temp->totalscore = tscore;
        temp->isValid = true;
    
        // Storing the information in the file.
        fwrite(temp, sizeof(student), 1, fp);
        printf("ADDED SUCCESSFULLY");
    
        free(temp);
        return;
    }
    
    void search(FILE *fp)
    {
    
        int roll;
        student *temp = (student *)malloc(sizeof(student));
    
        rewind(fp);
    
        printf("Enter RollNo To be Searched: ");
        scanf(" %d", &roll);
    
        while (fread(temp, sizeof(student), 1, fp))
        {
        
            // If the inforamation of the student is valid.
            if (temp->isValid == true)
            {
                // If the inforamation of the student is valid.
                if (temp->roll == roll)
                {
                    printf("Name-%s, Roll-%d, TotalScore-%d", temp->name, temp->roll, temp->totalscore);
                    printf("\n");
                    free(temp);
                    return;
                }
            }
        }
        free(temp);
        printf("The rollno dosen't exist\n");
    }
    
    void modifyInfo(FILE *fp)
    {
    
        int roll, i, tscore = 0, tempmarks = 0, size;
        char chk;
    
        student *temp = (student *)malloc(sizeof(student));
        rewind(fp);
    
        printf("Enter the roll to be modified: \n");
        scanf(" %d", &roll);
    
        while (fread(temp, sizeof(student), 1, fp))
        {
        
            if (temp->isValid)
            {
            
                if (temp->roll == roll)
                {
                   
                    printf("\n@To modify name press 'n'\n@To modify marks press 's'\n");
                    scanf(" %c", &chk);
    
                    switch (chk)
                    {
                        // This case is to modify the the name.
    
                    case 'n':
    
                        printf("Enter the modified name: \n");
                        scanf(" %s", temp->name);
                        break;
    
                    // This case is to modify the subject marks.
                    case 's':
    
                        for (i = 0; i < 5; i++)
                        {
                        
                            printf("Enter the marks of sub-%d\n", i + 1);
                            scanf(" %d", &tempmarks);
    
                            tscore += tempmarks;
                            temp->marks[i] = tempmarks;
                        }
    
                        temp->totalscore = tscore;
    
                        break;
                    }
    
                    size = (int)sizeof(student);
    
                    // Moving the pointer to the prev location to entered the modified value at it's previous position.
                    fseek(fp, -size, SEEK_CUR);
    
                    fwrite(temp, sizeof(student), 1, fp);
                    free(temp);
    
                    printf("\nModified Successfully....\n");
                    return;
                }
            }
        }
    
        printf("Roll no is not exist\n");
    
        free(temp);
    }
    
    void delete(FILE *fp)
    {
    
        int roll, count = 0, size, filesize;
        bool exist = false;
    
        printf("Enter the roll no to be deleted: \n");
        scanf("%d", &roll);
    
        student *temp = (student *)malloc(sizeof(student));
    
        rewind(fp);
    
        // Loop for finding the roll no in the file to be deleted.
        while (fread(temp, sizeof(student), 1, fp))
        {
        
            if (temp->roll == roll && temp->isValid)
            {
                exist = true;
                temp->isValid = false;
                size = (int)sizeof(student);
                fseek(fp, -size, 1);
                fwrite(temp, sizeof(student), 1, fp);
                break;
            }
        }
    
        // if the roll is not found.
        if (!exist)
        {
        
            printf("The Rollno Dosen't Exist");
            return;
        }
    
        rewind(fp);
    
        // getting the invalid information of the file.
        while (fread(temp, sizeof(student), 1, fp))
        {
        
            if (temp->isValid == false)
            {
                count++;
            }
        }
        fseek(fp, 0, 2);
    
        filesize = ftell(fp) / sizeof(student);
    
        // if the invalid no of element is >= the no of total element in the file, the all the information will be deleted.
        if (count >= filesize / 2)
        {
        
            // Creating a temporary file to copy the file info.
            FILE *tempfile = fopen("temp.bin", "w+");
            rewind(fp);
    
            while (fread(temp, sizeof(student), 1, fp))
            {
                // copying all the students information from the main file to temp file.
                fwrite(temp, sizeof(student), 1, tempfile);
            }
    
            rewind(tempfile);
            FILE *fpp = fopen("Student.bin", "w");
            while (fread(temp, sizeof(student), 1, tempfile))
            {
            
                // copying only the valid students information from the temp file to main file
                if (temp->isValid)
                {
                
                    fwrite(temp, sizeof(student), 1, fpp);
                }
            }
            // deleting the temp file
            remove("temp.bin");
    
            // closing the file pointer
            fclose(tempfile);
            fclose(fpp);
        }
    
        printf("DELETED SUCCESSFULLY");
    }
    
    // Function to display the valid information stored in the file.
    void display(FILE *fp)
    {
    
        int filesize;
    
        fseek(fp, 0, 2);
    
        // getting the no of elements in the file.
        filesize = ftell(fp) / sizeof(student);
    
        //printf("\nThe Elements in the file is: %d\n", filesize);
    
        rewind(fp);
    
        student *temp = (student *)malloc(sizeof(student));
    
        printf("\n");
    
        // printing all the students with their information
        while (fread(temp, sizeof(student), 1, fp))
        {
            // printing all the valid students information
            if (temp->isValid == true)
            {
            
                printf("Name-%s, Roll-%d, TotalScore-%d", temp->name, temp->roll, temp->totalscore);
                printf("\n");
            }
        }
        printf("\n");
    
        free(temp);
    }
    
    void main()
    {
    
        FILE *fp = NULL; 
        fp = fopen("Student.bin", "r+");
        char chk, brk;
    
        while (1)
        {
        
            printf("\n1.To add new student press '+'\n2.To Display press 'd'\n3.To Search press 's' \n4.To modify press 'm'\n5.To Delete press '-'\n");
            scanf(" %c", &chk);
    
            if (chk == '+')
            {
            
                addStudent(fp);
            }
            else if (chk == 'd')
            {
            
                display(fp);
                // printf("\nPrinting all the info\n");
                // display2(fp);
            }
            else if (chk == 's')
            {
            
                search(fp);
            }
            else if (chk == 'm')
            {
            
                modifyInfo(fp);
            }
            else if (chk == '-')
            {
            
                delete (fp);
            }
    
            printf("\n");
            printf("Want to Continue...?(y/n) :  ");
            scanf(" %c", &brk);
    
            if (brk == 'n')
            {
                break;
            }
        }
    }