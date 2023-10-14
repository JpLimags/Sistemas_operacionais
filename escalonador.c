#include <stdio.h>

#define MAX_PROCESSES 10

typedef struct {
    int process_id;
    int burst_time;
    int remaining_burst_time;
    int waiting_time;
    int turnaround_time;
} Process;

void round_robin_scheduler(Process processes[], int n, int quantum) {
    int remaining_burst_time[MAX_PROCESSES];
    int completed = 0;
    int current_time = 0;

    for (int i = 0; i < n; i++) {
        remaining_burst_time[i] = processes[i].burst_time;
    }

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (remaining_burst_time[i] > 0) {
                if (remaining_burst_time[i] <= quantum) {
                    current_time += remaining_burst_time[i];
                    remaining_burst_time[i] = 0;
                    completed++;
                    processes[i].waiting_time = current_time - processes[i].burst_time;
                    processes[i].turnaround_time = current_time;
                } else {
                    current_time += quantum;
                    remaining_burst_time[i] -= quantum;
                }
                printf("Processo %d executado por %d unidades de tempo\n", processes[i].process_id, quantum);
            }
        }
    }
}

void calculate_metrics(Process processes[], int n, float *average_waiting_time, float *average_turnaround_time, float *throughput) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    *throughput = (float)n / processes[n - 1].turnaround_time;

    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    *average_waiting_time = (float)total_waiting_time / n;
    *average_turnaround_time = (float)total_turnaround_time / n;
}

int main() {
    int n = 5;  // Número de processos
    int burst_times[] = {10, 8, 12, 5, 9};  // Burst time para cada processo
    int quanta[] = {1, 2, 4};  // Valores de quantum

    for (int q = 0; q < sizeof(quanta) / sizeof(quanta[0]); q++) {
        printf("Round Robin com Quantum = %d\n", quanta[q]);
        Process processes[MAX_PROCESSES];
        for (int i = 0; i < n; i++) {
            processes[i].process_id = i + 1;
            processes[i].burst_time = burst_times[i];
        }

        round_robin_scheduler(processes, n, quanta[q]);

        float average_waiting_time, average_turnaround_time, throughput;
        calculate_metrics(processes, n, &average_waiting_time, &average_turnaround_time, &throughput);

        printf("Métricas para Quantum = %d:\n", quanta[q]);
        printf("Tempo Médio de Espera: %.2f\n", average_waiting_time);
        printf("Tempo Médio de Retorno: %.2f\n", average_turnaround_time);
        printf("Vazão: %.2f\n\n", throughput);
    }

    return 0;
}
