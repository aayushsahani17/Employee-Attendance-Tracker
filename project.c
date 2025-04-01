#include<stdio.h>
#include<string.h>
typedef struct {
    int id;
    char name[50];
    char department[20];
    long contact;
} info;
typedef struct {
    int id;
    char date[11];
    char inTime[6];
    char outTime[6];
} attendance;
void menuOption() {
    printf("\n----Menu Option----\n");
    printf("1. Add Employee\n");
    printf("2. Mark Attendance\n");
    printf("3. View Attendance Report\n");
    printf("4. Edit Attendance\n");
    printf("5. Delete Attendance\n");
    printf("6. Exit\n");
    printf("-------------------\n");
}
void addEmployee() {
    FILE *eat = fopen("employee.txt", "a");
    if (eat == NULL) {
        printf("Error in File\n");
        return;
    }
    int size;
    printf("\nEnter the number of Employees: ");
    scanf("%d", &size);
    info x[size];
    for (int i = 0; i < size; i++) {
        printf("\nEnter the Id: ");
        scanf("%d", &x[i].id);
        getchar();
        printf("Enter the Name: ");
        fgets(x[i].name, 50, stdin);
        x[i].name[strcspn(x[i].name, "\n")] = '\0';
        printf("Enter the Department: ");
        scanf("%s",x[i].department);
        printf("Enter the Contact No.: ");
        scanf("%ld", &x[i].contact);
        fprintf(eat, "%d %s %s %ld\n", x[i].id, x[i].name, x[i].department, x[i].contact);
    }
    fclose(eat);
    printf("\n--Employee Added Successfully.--\n");
}
void markAttendance() {
    FILE *eat = fopen("employee.txt", "r");
    if (eat == NULL) {
        printf("Error in File\n");
        return;
    }
    FILE *att = fopen("attendance.txt", "a");
    if (att == NULL) {
        printf("Error in Attendance File\n");
        fclose(eat);
        return;
    }
    int found = 0;
    info x;
    int xid;
    printf("Enter the Id: ");
    scanf("%d", &xid);
    char line[200];
    while (fgets(line, sizeof(line), eat)) {
        sscanf(line, "%d %s %s %ld", &x.id, x.name, x.department, &x.contact);
        if (xid == x.id) {
            printf("Name: %s\n", x.name);
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Employee Not Found\n");
    } else {
        attendance a;
        a.id = xid;
        printf("Enter the Date (YYYY-MM-DD): ");
        scanf("%s", a.date);
        printf("Enter the In Time (HH:MM): ");
        scanf("%s", a.inTime);
        printf("Enter the Out Time (HH:MM): ");
        scanf("%s", a.outTime);
        fprintf(att, "%d %s %s %s\n", a.id, a.date, a.inTime, a.outTime);
        printf("\nAttendance Marked Successfully\n");
    }
    fclose(eat);
    fclose(att);
}
void attendanceReport() {
    FILE *att = fopen("attendance.txt", "r");
    FILE *emp = fopen("employee.txt", "r");
    if (att == NULL) {
        printf("No attendance records found.\n");
        return;
    }
    if (emp == NULL) {
        printf("Employee records not found.\n");
        fclose(att);
        return;
    }
    attendance a;
    info x;
    int xid, found = 0;
    printf("Enter the Employee ID: ");
    scanf("%d", &xid);
    while (fscanf(emp, "%d %s %s %ld", &x.id, x.name, x.department, &x.contact) == 4) {
        if (xid == x.id) {
            printf("\nName: %s\n", x.name);
            printf("Date         In Time  Out Time\n");
            printf("--------------------------------\n");
            break;
        }
    }
    while (fscanf(att, "%d %s %s %s", &a.id, a.date, a.inTime, a.outTime) == 4) {
        if (xid == a.id) {
            printf("%s    %s     %s\n", a.date, a.inTime, a.outTime);
            found = 1;
        }
    }
    if (!found) {
        printf("No attendance records found for this employee.\n");
    }
    fclose(att);
    fclose(emp);
}
void editAttendance() {
    FILE *att = fopen("attendance.txt", "r");
    if (att == NULL) {
        printf("No attendance records found.");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error file.");
        return;
    }

    attendance a;
    int xid;
    char editDate[11];
    int found = 0;

    printf("Enter Employee ID: ");
    scanf("%d", &xid);
    printf("Enter Date (YYYY-MM-DD) to Edit: ");
    scanf("%s", editDate);

    while (fscanf(att, "%d %s %s %s", &a.id, a.date, a.inTime, a.outTime) == 4) {
        if (a.id == xid && strcmp(a.date, editDate) == 0) {
            printf("Current In Time: %s\n", a.inTime);
            printf("Current Out Time: %s\n", a.outTime);
            printf("Enter New In Time (HH:MM): ");
            scanf("%s", a.inTime);
            printf("Enter New Out Time (HH:MM): ");
            scanf("%s", a.outTime);
            found = 1;
        }
        fprintf(temp, "%d %s %s %s\n", a.id, a.date, a.inTime, a.outTime);
    }
    if (!found) {
        printf("Attendance record not found for ID %d on %s.\n", xid, editDate);
        remove("temp.txt");
    } else {
        remove("attendance.txt");
        rename("temp.txt", "attendance.txt");
        printf("Attendance updated successfully.\n");
    }
    fclose(att);
    fclose(temp);
}
void deleteAttendanceRecord() {
    FILE *att = fopen("attendance.txt", "r");
    if (att == NULL) {
        printf("No attendance records found.\n");
        return;
    }
    attendance records[1000];
    int count = 0;
    attendance a;
    int xid;
    char deleteDate[11];
    int found = 0;
    printf("Enter Employee ID: ");
    scanf("%d", &xid);
    printf("Enter Date (YYYY-MM-DD) to Delete: ");
    scanf("%s", deleteDate);
    while (fscanf(att, "%d %s %s %s", &a.id, a.date, a.inTime, a.outTime) == 4) {
        if (a.id == xid && strcmp(a.date, deleteDate) == 0) {
            printf("Attendance record for Employee ID %d on %s deleted successfully.\n", xid, deleteDate);
            found = 1;
        } else {
            records[count++] = a;
        }
    }
    fclose(att);
    if (!found) {
        printf("No matching attendance record found.\n");
        return;
    }
    att = fopen("attendance.txt", "w");
    if (att == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(att, "%d %s %s %s\n", records[i].id, records[i].date, records[i].inTime, records[i].outTime);
    }

    fclose(att);
}
void exitProgram() {
    printf("    Goodbye\n");
    printf("--  Exiting  --\n");
}
int main() {
    int option;
    do {
        menuOption();
        printf("\nEnter the option: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                addEmployee();
                break;
            case 2:
                markAttendance();
                break;
            case 3:
                attendanceReport();
                break;
            case 4:
                editAttendance();
                break;
            case 5:
                deleteAttendanceRecord();
                break;
            case 6:
                exitProgram();
                break;
            default:
                printf("\nInvalid option. Please try again.\n");
                break;
        }
    } while (option != 6);
    return 0;
}
