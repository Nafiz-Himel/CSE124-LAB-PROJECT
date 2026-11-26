# Smoke Test Checklist

Build:
- `gcc -Wall -Wextra -Iinclude src/main.c src/utils.c src/reports.c src/dept_batch_section.c src/course.c src/registration.c src/section.c -o bin/app.exe`

Admin:
- Login admin/admin123
- Manage Batches/Sections: add, view, delete guarded by dependencies
- Add Course/Prerequisite, view, delete guarded by registrations/waitlist
- Manage Section Requests: view pending, approve/reject/offer alternative
- Manage Registration: register/drop/view/waitlists
- Reports: generate summaries and export named report

Student:
- Login with seeded student
- View available sections, submit preference, view status
- Accept/reject alternative offer
- Register/drop course, view credits/waitlist/history

Advisor:
- Login with seeded advisor
- View assigned students and their registration (scoped)

Data files:
- Verify updates in `data/*.txt` and exports in `reports/`.
