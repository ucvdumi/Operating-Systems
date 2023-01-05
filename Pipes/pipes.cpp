#include <iostream>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// Function to check if a number is prime or not
bool isPrime(int n)
{
    if (n <= 1)
        return false;

    for (int i = 2; i <= sqrt(n); i++)
        if (n % i == 0)
            return false;

    return true;
}

int main()
{
    int p[10][2]; // Array of pipes

    // Create 10 pipes
    for (int i = 0; i < 10; i++)
        pipe(p[i]);

    // Create 10 child processes
    for (int i = 0; i < 10; i++)
    {
        if (fork() == 0)
        {
            // Child process

            // Close the write end of the pipe
            close(p[i][1]);

            // Read the interval from the pipe
            int start, end;
            read(p[i][0], &start, sizeof(start));
            read(p[i][0], &end, sizeof(end));

            // Close the read end of the pipe
            close(p[i][0]);

            // Search for prime numbers in the given interval
            for (int j = start; j <= end; j++)
                if (isPrime(j))
                    cout << j << " ";

            // Exit the child process
            exit(0);
        }
        else
        {
            // Parent process

            // Close the read end of the pipe
            close(p[i][0]);

            // Calculate the interval to search for prime numbers
            int start = i * 1000 + 1;
            int end = (i + 1) * 1000;

            // Write the interval to the pipe
            write(p[i][1], &start, sizeof(start));
            write(p[i][1], &end, sizeof(end));

            // Close the write end of the pipe
            close(p[i][1]);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < 10; i++)
        wait(NULL);

    return 0;
}
