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

//iniデータ
int isauto_input = 0;
char input_filename[100];
char output_filename[100];
int ismessage = 1;
int islog = 1;

int isdelterm = 0;

const char alphabets[27+10] = "0123456789abcdefghijklmnopqrstuvwxyz";

int slidenum = 0;
slidechannel sld[26];

FILE* fp, * fp_out;
fpos_t fpos;

char buffer1[1920 * 2 + 1 + 8];
char buffer2[1920 * 2 + 1 + 8];
char buffer3[1920 * 2 + 1 + 8];
char buffer4[1920 * 2 + 1 + 8];
char buffer5[1920 * 2 + 1 + 8];
char buf;

char type_info_tmp[5 + 5 + 1];
char notes_tmp[1920 * 2 + 1];

char type_info_tmp2[5 + 5 + 1];
char notes_tmp2[1920 * 2 + 1];

char tmp[2 + 1];
int division;
char sectmp[3 + 1];

slidemod modarray[50000 + 1];
int modnum = 0, modout = 0;

//input
int howMove = 0;
int tmpPos = 0;
char tmpc[10];

void clrbuf(void) {
	for (int i = 0; i < 1920 * 2 + 1 + 8; i++) {
		buffer1[i] = 0;
		buffer2[i] = 0;
		buffer3[i] = 0;

	}
	for (int i = 0; i < 1920 * 2; i++) {
		notes_tmp2[i] = '0';
	}
	notes_tmp2[0] = 0;
}


int main(int argc, char* argv[]) {

	if (argc == 1) {
		printf("<info> susファイルをこのexeにドラッグアンドドロップしてください\n");
		system("PAUSE");
		exit(0);
	}

	/*
	//ini読み込み
	if ((fp = fopen("suspatcher_setting.ini", "r")) == NULL) {
		//ファイルが存在しないので新たに作る

		if ((fp = fopen("suspatcher_setting.ini", "w")) == NULL) {
			printf("<error>iniの生成中に予期しないエラーが発生しました。もう一度お試しください。\n");
			system("PAUSE");
			exit(1);
		}
		else {
			if (argc == 1) {
				printf("<info> susファイルをこのexeにドラッグアンドドロップしてください\n");
				system("PAUSE");
				exit(0);
			}

			strcpy(input_filename, argv[1]);
			if (argc >= 3) {
				strcpy(output_filename, argv[2]);
			}
			else {
				strcpy(output_filename, "patched.sus");
			}
			printf("どのくらいずらしますか？\n>>>");
			scanf("%d", &howMove);
			
			fprintf(fp, "[input]\n");
			fprintf(fp, "isauto input = %d\n", isauto_input);
			fprintf(fp, "input filename = %s\n", input_filename);
			fprintf(fp, "\n[output]\n");
			fprintf(fp, "output filename = %s\n", output_filename);
			fprintf(fp, "\n[other_settings]\n");
			fprintf(fp, "ismessage = %d\n", ismessage);
			fprintf(fp, "forward logs = %d\n", islog);
			fprintf(fp, "How Move = %d\n", howMove);
			//fprintf(fp, "isdel term = %d\n", isdelterm);
		}
	}
	else {
		//ファイル存在しているのでデータ読む
		fscanf(fp, "%[^\n]%c", buffer1, &buf);

		fscanf(fp, "%[^\n]%c", buffer1, &buf);
		sscanf(buffer1, "%[^0-9]%d", buffer2, &isauto_input);

		fscanf(fp, "%[^\n]%c", buffer1, &buf);
		sscanf(buffer1, "%s %s %s %s", buffer2, buffer3, buffer4, input_filename);

		fgetc(fp);
		fscanf(fp, "%[^\n]%c", buffer1, &buf);

		fscanf(fp, "%[^\n]%c", buffer1, &buf);
		sscanf(buffer1, "%s %s %s %s", buffer2, buffer3, buffer4, output_filename);

		fgetc(fp);
		fscanf(fp, "%[^\n]%c", buffer1, &buf);

		fscanf(fp, "%[^\n]%c", buffer1, &buf);
		sscanf(buffer1, "%[^0-9]%d", buffer2, &ismessage);

		fscanf(fp, "%[^\n]%c", buffer1, &buf);
		sscanf(buffer1, "%[^0-9]%d", buffer2, &islog);

		fscanf(fp, "%[^\n]%c", buffer1, &buf);
		sscanf(buffer1, "%[^0-9]%d", buffer2, &howMove);

		//fscanf(fp, "%[^\n]%c", buffer1, &buf);
		//sscanf(buffer1, "%[^0-9]%d", buffer2, &isdelterm);

	}
	fclose(fp);
	//ここまで
	*/
	printf("どのくらいずらしますか？\n>>>");
	scanf("%d", &howMove);
	printf("出力ファイルの名前？\n>>>");
	scanf("%s", output_filename);

	//start of input part
	if (!isauto_input) {
		if (argc == 1) {
			printf("<info> susファイルをこのexeにドラッグアンドドロップするか、iniファイルを書いてください。\n<info>今回はデフォルト設定としてoutput.susを読み込みます。\n");
			strcpy(input_filename, "output.sus");
			//system("PAUSE");
			//exit(0);
		}
		else {
			strcpy(input_filename, argv[1]);
		}
	}

	if (ismessage) {
		printf("<info>%sを読み込みます\n", input_filename);
	}
	if ((fp = fopen(input_filename, "r")) == NULL) {
		printf("<error> ファイルの読み込みに失敗しました。fuzzy_setting.iniの内容を確認してください。\n繰り返しこのエラーが出る場合は\n1D4B42#0469\nまでお問い合わせください。\n");
		system("PAUSE");
		exit(1);
	}
	//end of input part

	//start of output part
	if ((fp_out = fopen(output_filename, "w")) == NULL) {
		printf("<error> ファイルの準備に失敗しました。iniファイルを一度削除するとこの問題が解決することがあります。\n繰り返しこのエラーが出る場合は\n1D4B42#0469\nまでお問い合わせください。\n");
		system("PAUSE");
		exit(1);
	}
	//end of output part

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
		for (int i = 0; i < 4; i++) {
			type_info_tmp[i] = fgetc(fp);
			//fprintf(fp_out, "%c ", type_info_tmp[i]);
		}
		type_info_tmp[4] = 0;
		//追加
		tmpc[0] = fgetc(fp);tmpc[1] = 0;
		for (int i = 0; i < 27 + 10; i++) {
			if (tmpc[0] == alphabets[i]) {
				i += howMove;
				type_info_tmp[4] = alphabets[i];
				break;
			}
		}
		//tmpPos += howMove;
		//if (type_info_tmp[4] == 0)continue;
		

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

	if (ismessage) {
		printf("<info> ファイルの変換が完了しました!\n入力ファイルと同じディレクトリに%sが作成されているか確認してください\n", output_filename);
		system("PAUSE");
	}
	return 0;
}