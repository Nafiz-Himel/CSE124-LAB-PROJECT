# Smoke Test Checklist

Build (from project root):
- `gcc -Wall -Wextra -Iinclude src/main.c src/utils.c src/reports.c src/dept_batch_section.c src/course.c src/registration.c src/section.c -o bin/app.exe`
- Back up `data/` manually before destructive tests; demo TXT files are maintained directly.

Admin:
- Login `A001` / `admin001`
- Manage Batches/Sections: add, view, delete guarded by dependencies; new sections always get capacity 40
- Add Course/Prerequisite, view, delete guarded by registrations/waitlist; courses show semester 1-12, not capacity
- Update Student: change current semester; verify it appears in profile and reports
- Manage Registration: register/drop/view/waitlists; verify registration rows include section
- View all registration requests
- Reports: generate summaries and export named report; course report shows per-section fill

Student:
- Login with a seeded student (`261-15-001` / `student001`)
- View profile with batch, section, and current semester
- View available sections, submit preference, view status
- Accept/reject alternative offer
- Submit registration request, view request status
- Try an ineligible-semester course and confirm rejection
- Try to exceed 18 credits with pending requests and confirm rejection
- Drop course, view credits/waitlist/history

Advisor:
- Login with a seeded advisor (`ADV001` / `advisor001`)
- View assigned students and their registration (scoped)
- View pending registration requests (assigned students only)
- Approve a request; verify registration created in the student's section
- Reject a request; verify status and history entry

Data files:
- Verify updates in `data/*.txt` and exports in `reports/`
- Confirm `data/courses.txt` is `code|name|credits|semester`
- Confirm `data/registrations.txt` is `studentID|courseCode|section|status`
- Confirm `data/history.txt` is `studentID|action|courseCode|section`
