# Student Course Registration System

CSE124 - Data Structure Lab project: a console-based course registration system in C.

## Data Structures Used

| Module | Data Structure | Purpose |
|---|---|---|
| Admin/Student/Advisor records | Singly Linked List | Dynamic in-memory record management |
| Departments/Batches/Sections/Courses/Prerequisites | Singly Linked List | Catalog and organization data |
| Course Registration | Singly Linked List | Registration records and status tracking |
| Waitlist | Queue (FIFO) | Fair course waitlist processing |
| Registration History | Stack (LIFO) | Recent action/history tracking |

## Folder Structure

```text
student-course-registration-system/
├── .gitignore
├── Makefile
├── README.md
├── bin/                    # Compiled executable output
├── build/                  # Build artifacts (optional)
├── data/                   # Persistent text data files
├── docs/                   # Documentation/proposal
├── include/                # Header files
├── reports/                # Generated report exports
└── src/                    # C source files
```

## Build

```powershell
gcc -Wall -Wextra -Iinclude src/main.c src/reports.c src/dept_batch_section.c src/course.c src/registration.c -o bin/app.exe
```

Or with Make (if available):

```powershell
make
```

## Run

```powershell
.\bin\app.exe
```

Run from the `student-course-registration-system` directory so relative `data/` and `reports/` paths resolve correctly.

## Data Files

- `data/admins.txt`
- `data/advisors.txt`
- `data/students.txt`
- `data/departments.txt`
- `data/batches.txt`
- `data/sections.txt`
- `data/courses.txt`
- `data/prerequisites.txt`
- `data/registrations.txt`
- `data/waitlists.txt`
- `data/history.txt`
