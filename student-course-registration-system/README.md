# Student Course Registration System

CSE124 - Data Structure Lab project: a console-based course registration system in C for CSE department students only.

## Implemented Modules
- Role-based login (Admin/Student/Advisor)
- Batch / Section management (LL, TXT persistence, fixed 40-student section capacity)
- Course & prerequisite management (LL, explicit 12-semester offering map)
- Registration request workflow (student submit, advisor approve/reject, scoped to assigned students)
- Registration, waitlist, and history (LL + Queue FIFO + Stack LIFO, keyed by course + section)
- Section request workflow (submit, approve/reject/alternative, change request)
- Reports and export (`reports/last_report.txt`, named export)

## Rules
- A student can request only courses offered in their current semester.
- Registered + pending credits must not exceed 18.0 per semester.
- Prerequisites must be satisfied by REGISTERED or COMPLETED courses.
- Course capacity is section-wise: each batch section has capacity 40; no course-wide capacity exists.
- Admin sets/promotes a student's current semester and may override registration with the same validation.

## Data Structures
- **Singly Linked List**: admins, students, advisors, batches, sections, courses, prerequisites, registrations, registration requests, section requests
- **Queue (FIFO)**: per-course-per-section waitlist
- **Stack (LIFO)**: registration/section action history

No Hash Table, BST, or Directed Graph used.

## Build & Run
```powershell
gcc -Wall -Wextra -Iinclude src/main.c src/utils.c src/reports.c src/dept_batch_section.c src/course.c src/registration.c src/section.c -o bin/app.exe
./bin/app.exe
```
Or (requires GNU make on PATH):
```powershell
make
./bin/app.exe
```
Run from `student-course-registration-system` so relative `data/` and `reports/` paths resolve.

## Data Files
- `data/admins.txt`, `data/students.txt`, `data/advisors.txt`
- `data/batches.txt`, `data/sections.txt`
- `data/courses.txt`, `data/prerequisites.txt`
- `data/registrations.txt`, `data/waitlists.txt`, `data/history.txt`
- `data/registration_requests.txt`, `data/section_requests.txt`

Demo TXT files are maintained directly in `data/`; no external generator is required. Back up `data/` before destructive tests.

## Notes
- Input uses validated helpers in `src/utils.c`.
- Mutating operations save to TXT files.
- Runtime executable launch may be blocked on systems with Device Guard policy; compilation is supported via GCC.
