#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For the sleep function

void run_command(const char *command) {
    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Error executing command: %s\n", command);
        // exit(1);
    }
}

int has_changes() {
    int result = system("git diff-index --quiet HEAD --");
    return result; // 0 means no changes, non-zero means there are changes
}

void auto_commit() {
    if (has_changes() != 0) {
        // Generate a commit message with the current date and time
        char commit_message[128];
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(commit_message, sizeof(commit_message), "Auto-commit on %Y-%m-%d %H:%M:%S", t);

        // Stage all changes
        printf("Staging changes...\n");
        run_command("git add .");

        // Commit changes
        char git_commit_command[256];
        snprintf(git_commit_command, sizeof(git_commit_command), "git commit -m \"%s\"", commit_message);
        printf("Committing with message: %s\n", commit_message);
        run_command(git_commit_command);

        // Push changes
        printf("Pushing changes...\n");
        run_command("git push");

        printf("Auto-commit completed successfully.\n");
    } else {
        printf("No changes to commit. Skipping...\n");
    }
}

// void auto_commit() {
//     // Generate a commit message with the current date and time
//     char commit_message[128];
//     time_t now = time(NULL);
//     struct tm *t = localtime(&now);
//     strftime(commit_message, sizeof(commit_message), "Auto-commit on %Y-%m-%d %H:%M:%S", t);

//     // Stage all changes
//     printf("Staging changes...\n");
//     run_command("git add .");

//     // Commit changes
//     char git_commit_command[256];
//     snprintf(git_commit_command, sizeof(git_commit_command), "git commit -m \"%s\"", commit_message);
//     printf("Committing with message: %s\n", commit_message);
//     run_command(git_commit_command);

//     // Push changes
//     printf("Pushing changes...\n");
//     run_command("git push");

//     printf("Auto-commit completed successfully.\n");
// }

int main() {
    printf("Starting auto-commit loop (every 10 seconds)...\n");

    while (1) {
        auto_commit();
        printf("Waiting 6 seconds for the next commit...\n");
        sleep(6); // Pause for 6 seconds
    }

    return 0;
}
