# Student Course Registration System

CSE124 - Data Structure Lab project: a console-based Student Course Registration System in C.

## Implemented Modules
- Role-based login (Admin/Student/Advisor)
- Department / Batch / Section management (LL, TXT persistence)
- Course & prerequisite management (LL, dependency guards)
- Registration, waitlist, and history (LL + Queue FIFO + Stack LIFO)
- Section request workflow (submit, approve/reject/alternative, change request)
- Reports and export (`reports/last_report.txt`, named export)

## Data Structures
- **Singly Linked List**: admins, students, advisors, departments, batches, sections, courses, prerequisites, registrations, section requests
- **Queue (FIFO)**: per-course waitlist
- **Stack (LIFO)**: registration/section action history

No Hash Table, BST, or Directed Graph used.

## Build & Run
```powershell
gcc -Wall -Wextra -Iinclude src/main.c src/utils.c src/reports.c src/dept_batch_section.c src/course.c src/registration.c src/section.c -o bin/app.exe
.inpp.exe
```
Or:
```powershell
make
.inpp.exe
```
Run from `student-course-registration-system` so relative `data/` and `reports/` paths resolve.

## Data Files
- `data/admins.txt`, `data/students.txt`, `data/advisors.txt`
- `data/departments.txt`, `data/batches.txt`, `data/sections.txt`
- `data/courses.txt`, `data/prerequisites.txt`
- `data/registrations.txt`, `data/waitlists.txt`, `data/history.txt`
- `data/section_requests.txt`

## Notes
- Input uses validated helpers in `src/utils.c`.
- Mutating operations save to TXT files.
- Runtime executable launch may be blocked on systems with Device Guard policy; compilation is supported via GCC.
