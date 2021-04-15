#include <stdio.h>
#include <stdlib.h>
#include <time.h>


const int messageLength = 1023;
const int bitfieldLength = 10;
const int combinationsLength = 24;

int readFile(const char *fileName, int *data) {
    FILE *file;

    if (fopen_s(&file, fileName, "r") != 0) {
        perror("File does not exists!");
        return -1;
    }

    int value = 0;

    for (int i = 0; i < messageLength; i++) {
        fscanf_s(file, "%d", &value);
        data[i] = value;
    }

    fclose(file);
    return 0;
}

void episodeOne(int *episode) {
    int bitfield[bitfieldLength];
    for (int i = 0; i < bitfieldLength; i++) {
        bitfield[i] = 1;
    }
    for (int i = 0; i < messageLength; i++) {
        episode[i] = bitfield[9];
        int rotateBit = bitfield[2] ^bitfield[9];
        for (int j = bitfieldLength - 1; j > 0; j--) {
            bitfield[j] = bitfield[j - 1];
        }
        bitfield[0] = rotateBit;
    }
}

void episodeTwo(int *episode, int a, int b) {
    int bitfield[bitfieldLength];
    for (int i = 0; i < bitfieldLength; i++) {
        bitfield[i] = 1;
    }
    for (int i = 0; i < messageLength; i++) {
        episode[i] = bitfield[a] ^ bitfield[b];
        int rotateBit = bitfield[1] ^bitfield[2] ^bitfield[5] ^bitfield[7] ^bitfield[8] ^bitfield[9];
        for (int j = bitfieldLength - 1; j > 0; j--) {
            bitfield[j] = bitfield[j - 1];
        }
        bitfield[0] = rotateBit;
    }
}

void chipCodes(int chipSequences[][messageLength*2]) {
    int combinations[24][2] = {{1,5},{2,6},{3,7},{4,8},{0,8},{1,9},{0,7},{1,8},{2,9},{1,2},{2,3},{4,5},{5,6},{6,7},{7,8},{8,9},{0,3},{1,4},{2,5},{3,6},{4,7},{5,8},{0,2},{3,5}};
    int firstEpisode[messageLength];
    episodeOne(firstEpisode);
    for (int i = 0; i < combinationsLength; i++) {
        int secondEpisode[messageLength];
        episodeTwo(secondEpisode, combinations[i][0], combinations[i][1]);
        for (int k = 0; k < messageLength; k++) {
            secondEpisode[k] = secondEpisode[k] ^ firstEpisode[k];
            chipSequences[i][k] = secondEpisode[k];
            chipSequences[i][k+messageLength] = secondEpisode[k];
        }
    }
}


int main(int argc, char **argv) {

    clock_t start, stop;
    start = clock();

    if (argc < 2) {
        perror("File parameter is missing!\n");
        return -1;
    }

    int data[messageLength];
    if (readFile(argv[1], data)) {
        return -1;
    }
    int chipSequences[combinationsLength][messageLength*2];
    chipCodes(chipSequences);
    for (int id = 0; id < combinationsLength; id++) {
        for (int delta = 0; delta < messageLength; delta++) {
            int crossProduct = 0;
            for (int number = 0; number < messageLength; number++) {
                crossProduct += chipSequences[id][number+delta] * data[number];
            }

            if (200 < crossProduct) {
                printf("Satellite %2d has sent bit 1 (delta = %3d)\n", id + 1, delta + 1);
                break;
            } else if (-200 > crossProduct) {
                printf("Satellite %2d has sent bit 0 (delta = %3d)\n", id + 1, delta + 1);
                break;
            }
        }
    }

    stop = clock();
    printf("%.0f Milliseconds\n", ((double)(stop - start) / CLOCKS_PER_SEC) * 1000);
    return 0;
}

