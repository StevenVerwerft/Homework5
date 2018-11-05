#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define CRD 2500000

char hp[CRD][4], ahp[CRD][4], dis[CRD][4];
int code[39], adj[39], cnt[23][5];

void rules(void);
void init (void);
void init_set (void);
void output (void);

int main (void) {
	rules();
	clock_t start_time = clock();
	init ();
	init_set ();
	double iTime = ((double)(clock() - start_time)/CLK_TCK);	
	output ();
	printf("Computation time: %.2f seconds.\n", iTime);
	printf("Press any key to exit\n");
	getchar();
	return 0;
}

#define RU 100
int nrr, bid[RU], distl[RU], distu[RU], ahpl[RU], ahpu[RU], res[RU], color[RU][4], nrl[RU][4], nru[RU][4];

void rules(void) {
	FILE *fp;
	fp = fopen("rules.txt", "r");
	nrr = 0;
	for (int i = 0; i < RU; i++) {
		fscanf(fp, "%d", bid + i);
		if (bid[i] == 99) { nrr = i; break;}
		fscanf(fp, "%d %d %d %d %d", distl + i, distu + i, ahpl + i, ahpu + i, res + i);
		for (int j = 0; j < res[i]; j++)
			fscanf(fp, "%d %d %d", color[i] + j, nrl[i] + j, nru[i] + j);
	}
	fclose(fp);
	return;
}

void output (void) {
	FILE *fp;
	int cc[39][41], row[39], col[41], sum = 0;
	int a, b, c, d;
	for (int i = 0; i < 39; i++) {
		row[i] = 0;
		for (int j = 0; j < 41; j++)
			cc[i][j] = 0;
	}
	for (int j = 0; j < 41; j++)
		col[j] = 0;
	for (int i = 0; i < CRD; i++)
		for (int j = 0; j < 4; j++)
			++cc[dis[i][j]][hp[i][j]];
	fp = fopen("DIS_HP.txt","w");
	fprintf (fp, "Distribution versus HP\n\n              ");
	for (int i = 0; i < 41; i++)
		fprintf (fp, "%8d", i);
	fprintf (fp, "\n");
	for (int i = 0; i < 39; i++) {
		a = code[i]/35 + 4; b = (code[i]%35)/5; c = code[i]%5; d = 13 - a - b - c;
		fprintf (fp, "%2d (%d-%d-%d-%d):", i+1, a, b, c, d);
		if (i < 32) fprintf (fp, " ");
		for (int j = 0; j < 41; j++) {
			row[i] += cc[i][j];
			col[j] += cc[i][j];
			fprintf (fp, "%8d", cc[i][j]);
		}
		fprintf (fp, "%10d\n", row[i]);
	}
	fprintf (fp, "              ");
	for (int j = 0; j < 41; j++) {
		sum += col[j];
		fprintf (fp, "%8d", col[j]);
	}
	fprintf (fp, "%10d", sum);
	fclose (fp);
	for (int j = 0; j < 4; j++) {
		cnt[22][j] = 0;
		for (int i = 0; i < 22; i++) cnt[22][j] += cnt[i][j];
	}
	cnt[22][4] = 0;
	for (int i = 0; i < 22; i++) {
		printf("\n%2d", i);
		cnt[i][4] = 0;
		for (int j = 0; j < 4; j++) cnt[i][4] += cnt[i][j];
		for (int j = 0; j < 5; j++)
			printf("%12d", cnt[i][j]);
	}
	printf("\n22");
	cnt[22][4] = 0;
	for (int j = 0; j < 4; j++) cnt[22][4] += cnt[22][j];
	for (int j = 0; j < 5; j++)
		printf("%12d", cnt[22][j]);
	printf("\n\n");
	return;
}
