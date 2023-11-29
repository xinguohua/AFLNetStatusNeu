#include <stdio.h>

int unlock_step = 0;

void process_signal(int signal) {
    switch(unlock_step) {
        case 0:
            if (signal == 42) {
                unlock_step = 1;
            }
            break;
        case 1:
            if (signal == -42) {
                unlock_step = 2;
            } else {
                printf("Unexpected signal. Resetting...\n");
                unlock_step = 0;
            }
            break;
        case 2:
            if (signal == 0) {
                printf("Secret functionality unlocked!\n");
                char* ptr = NULL;
                *ptr = 'a';  // Intentional crash
            } else {
                printf("Unexpected signal. Resetting...\n");
                unlock_step = 0;
            }
            break;
        default:
            unlock_step = 0;
            break;
    }
}

int get_input_from_CMD() {
    int signal;
    printf("Enter signal: ");
    if (scanf("%d", &signal) == 1) {
        return signal;
    } else {
        // 输入无效时退出
        printf("Invalid input. Exiting...\n");
    }
}

int main() {
    printf("Starting communication device...\n");
    int a;
    while ( a = get_input_from_CMD( ) ) {
        process_signal (a);
    }

    return 0;
}
