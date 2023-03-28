#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
	char prev;
	char cur;
	int flag;
}slidechannel;

typedef struct {
	double num;
	int div;
	int type;
	char pos;
	char section[4];
}slidemod;

const char alphabets[27] = "abcdefghijklmnopqrstuvwxyz";
int slidenum = 0;
slidechannel sld[26];

FILE* fp, * fp_out;
fpos_t fpos;

char buffer1[1920 * 2 + 1 + 8];
char buf;

char type_info_tmp[6 + 1];
char notes_tmp[1920 * 2 + 1];

char type_info_tmp2[6 + 1];
char notes_tmp2[1920 * 2 + 1];

char tmp[2 + 1];
int division;
char sectmp[3 + 1];

slidemod modarray[50000 + 1];
int modnum = 0, modout = 0;

void clrbuf(void) {
	for (int i = 0; i < 1920 * 2 + 1 + 8; i++) {
		buffer1[i] = 0;
	}
}


int main(int argc, char* argv[]) {
	if (argc == 1) {
		printf("<info> susファイルをこのexeにドラッグアンドドロップしてください\n");
		system("PAUSE");
		exit(0);
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("<error> ファイルの読み込みに失敗しました。繰り返しこのエラーが出る場合は\n1D4B42#0469\nまでお問い合わせください。\n");
		system("PAUSE");
		exit(1);
	}
	if ((fp_out = fopen("output.sus", "w")) == NULL) {
		printf("<error> ファイルの読み込みに失敗しました。繰り返しこのエラーが出る場合は\n1D4B42#0469\nまでお問い合わせください。\n");
		system("PAUSE");
		exit(1);
	}
	clrbuf();

	while (1) {
		//各行の頭の位置を保存
		fgetpos(fp, &fpos);
		fgetc(fp); if (feof(fp))break;
		fsetpos(fp, &fpos);

		//譜面データと関係ないデータを排除
		if ((buf = fgetc(fp)) != '#') {
			if (buf == '\n') {
				fprintf(fp_out, "\n");
				continue;
			}
			fsetpos(fp, &fpos);
			fscanf(fp, "%[^\n]%*c", buffer1);
			fprintf(fp_out, "%s\n", buffer1);
			clrbuf();
			continue;
		}
		//タイプデータの読み込み
		for (int i = 0; i < 5; i++) {
			type_info_tmp[i] = fgetc(fp);
			//fprintf(fp_out, "%c ", type_info_tmp[i]);
		}
		//スライドデータである
		if (type_info_tmp[3] == '3') {
			type_info_tmp[5] = fgetc(fp);
			type_info_tmp[6] = 0;
		}
		else {//スライドデータではない
			type_info_tmp[5] = 0;
		}
		//notesデータでない
		if (!(type_info_tmp[0] >= '0' && type_info_tmp[0] <= '9') || type_info_tmp[3] == '0') {
			fsetpos(fp, &fpos);
			fscanf(fp, "%[^\n]%*c", buffer1);
			fprintf(fp_out, "%s\n", buffer1);
			clrbuf();
			continue;
		}
		//:を読み飛ばす
		fgetc(fp);

		//ここから
		//:以下の読み込み
		if (fgetc(fp) != ' ') {//スペースを挟むかどうか
			fseek(fp, -1, SEEK_CUR);
		}
		//:以下を格納
		fscanf(fp, "%s", notes_tmp);
		//fprintf(fp_out, "gotdata:%s\n", notes_tmp);

		//分割数
		division = (int)strlen(notes_tmp) / 2;
		//小節
		strncpy(sectmp, type_info_tmp, 3);

		for (int i = 0; i < division; i++) {
			tmp[0] = notes_tmp[2 * i];//type
			tmp[1] = notes_tmp[2 * i + 1];//size

			if (type_info_tmp[3] == '3') {

				for (modout = 0; modout < modnum; modout++) {
					//int flag = 0;

					if ((modarray[modout].pos == type_info_tmp[4])
						&& ((strcmp(modarray[modout].section, sectmp)) == 0)) {

						if (modarray[modout].num / modarray[modout].div == (double)i / division) {
							printf("sec1:%s sec2:%s a:%lf b:%lf type:%d index:%d\n",
								sectmp,
								modarray[modout].section,
								modarray[modout].num / modarray[modout].div,
								(double)i / division, modarray[modout].type,
								modout);

							if (modarray[modout].type == 3) {

								//不可視中継点にする
								if (tmp[0] != '3')notes_tmp[2 * i] = '5';
							}
							if (modarray[modout].type == 4) {
								//0幅にする
								notes_tmp[2 * i + 1] = '0';
							}
						}
						//printf("2\n");
					}
					//printf("1\n");
				}

			}
			else if (type_info_tmp[3] == '1') {
				switch (tmp[0]) {//種類に応じて操作対象を一時データ配列として保存
				case '3':

					//flick
					modarray[modnum].div = division;
					modarray[modnum].num = i;
					modarray[modnum].type = 3;
					modarray[modnum].pos = type_info_tmp[4];
					strcpy(modarray[modnum].section, sectmp);
					//modarray[modnum].section = sectmp;

					printf("flick:%d %lf %d %c %s :%d\n", modarray[modnum].div,
						modarray[modnum].num,
						modarray[modnum].type,
						modarray[modnum].pos,
						modarray[modnum].section,
						modnum);

					modnum++;

					//notes_tmp[2 * i] = '0';
					//notes_tmp[2 * i + 1] = '0';

					break;
				case '4':

					//damage
					modarray[modnum].div = division;
					modarray[modnum].num = i;
					modarray[modnum].type = 4;
					modarray[modnum].pos = type_info_tmp[4];
					strcpy(modarray[modnum].section, sectmp);
					//modarray[modnum].section = sectmp;

					printf("damage:%d %lf %d %c %s :%d\n", modarray[modnum].div,
						modarray[modnum].num,
						modarray[modnum].type,
						modarray[modnum].pos,
						modarray[modnum].section,
						modnum);

					notes_tmp[2 * i] = '0';
					notes_tmp[2 * i + 1] = '0';

					modnum++;
					break;
				}

			}


		}

		strcat(buffer1, "#");
		strcat(buffer1, type_info_tmp);
		strcat(buffer1, ":");
		strcat(buffer1, notes_tmp);
		fprintf(fp_out, "%s\n", buffer1);
		fgetc(fp);
		clrbuf();
	}
	fclose(fp); fclose(fp_out);
	printf("<info> ファイルの変換が完了しました!\n入力ファイルと同じディレクトリにoutput.susが作成されているか確認してください\n");
	system("PAUSE");
	return 0;
}