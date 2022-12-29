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

//ini�f�[�^
int isauto_input = 0;
char input_filename[100];
char output_filename[100];
int ismessage = 1;
int islog = 1;

const char alphabets[27] = "abcdefghijklmnopqrstuvwxyz";
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
		buffer2[i] = 0;
		buffer3[i] = 0;

	}
	for (int i = 0; i < 1920 * 2; i++) {
		notes_tmp2[i] = '0';
	}
	notes_tmp2[0] = 0;
}


int main(int argc, char* argv[]) {
	//ini�ǂݍ���
	if ((fp = fopen("suspatcher_setting.ini", "r")) == NULL) {
		//�t�@�C�������݂��Ȃ��̂ŐV���ɍ��
		if ((fp = fopen("suspatcher_setting.ini", "w")) == NULL) {
			printf("<error>ini�̐������ɗ\�����Ȃ��G���[���������܂����B������x���������������B\n");
			system("PAUSE");
			exit(1);
		}
		else {
			if (argc == 1) {
				printf("<info> sus�t�@�C��������exe�Ƀh���b�O�A���h�h���b�v���Ă�������\n");
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
			
			fprintf(fp, "[input]\n");
			fprintf(fp, "isauto input = %d\n", isauto_input);
			fprintf(fp, "input filename = %s\n", input_filename);
			fprintf(fp, "\n[output]\n");
			fprintf(fp, "output filename = %s\n", output_filename);
			fprintf(fp, "\n[other_settings]\n");
			fprintf(fp, "ismessage = %d\n", ismessage);
			fprintf(fp, "forward logs = %d\n", buffer1, buffer2, buffer3, islog);
		}
	}
	else {
		//�t�@�C�����݂��Ă���̂Ńf�[�^�ǂ�
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

	}
	fclose(fp);
	//�����܂�

	//start of input part
	if (!isauto_input) {
		if (argc == 1) {
			printf("<info> sus�t�@�C��������exe�Ƀh���b�O�A���h�h���b�v���邩�Aini�t�@�C���������Ă��������B\n<info>����̓f�t�H���g�ݒ�Ƃ���output.sus��ǂݍ��݂܂��B\n");
			strcpy(input_filename, "output.sus");
			//system("PAUSE");
			//exit(0);
		}
		else {
			strcpy(input_filename, argv[1]);
		}
	}

	if (ismessage) {
		printf("<info>%s��ǂݍ��݂܂�\n", input_filename);
	}
	if ((fp = fopen(input_filename, "r")) == NULL) {
		printf("<error> �t�@�C���̓ǂݍ��݂Ɏ��s���܂����Bfuzzy_setting.ini�̓��e���m�F���Ă��������B\n�J��Ԃ����̃G���[���o��ꍇ��\n1D4B42#0469\n�܂ł��₢���킹���������B\n");
		system("PAUSE");
		exit(1);
	}
	//end of input part

	//start of output part
	if ((fp_out = fopen(output_filename, "w")) == NULL) {
		printf("<error> �t�@�C���̏����Ɏ��s���܂����Bini�t�@�C������x�폜����Ƃ��̖�肪�������邱�Ƃ�����܂��B\n�J��Ԃ����̃G���[���o��ꍇ��\n1D4B42#0469\n�܂ł��₢���킹���������B\n");
		system("PAUSE");
		exit(1);
	}
	//end of output part

	clrbuf();

	while (1) {
		//�e�s�̓��̈ʒu��ۑ�
		fgetpos(fp, &fpos);
		fgetc(fp); if (feof(fp))break;
		fsetpos(fp, &fpos);

		//���ʃf�[�^�Ɗ֌W�Ȃ��f�[�^��r��
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
		//�^�C�v�f�[�^�̓ǂݍ���
		for (int i = 0; i < 5; i++) {
			type_info_tmp[i] = fgetc(fp);
			//fprintf(fp_out, "%c ", type_info_tmp[i]);
		}
		//�X���C�h�f�[�^�ł���
		if (type_info_tmp[3] == '3') {
			type_info_tmp[5] = fgetc(fp);
			type_info_tmp[6] = 0;
		}
		else {//�X���C�h�f�[�^�ł͂Ȃ�
			type_info_tmp[5] = 0;
		}
		//notes�f�[�^�łȂ�
		if (!(type_info_tmp[0] >= '0' && type_info_tmp[0] <= '9') || type_info_tmp[3] == '0') {
			fsetpos(fp, &fpos);
			fscanf(fp, "%[^\n]%*c", buffer1);
			fprintf(fp_out, "%s\n", buffer1);
			clrbuf();
			continue;
		}
		//:��ǂݔ�΂�
		fgetc(fp);

		//��������
		//:�ȉ��̓ǂݍ���
		if (fgetc(fp) != ' ') {//�X�y�[�X�����ނ��ǂ���
			fseek(fp, -1, SEEK_CUR);
		}
		//:�ȉ����i�[
		fscanf(fp, "%s", notes_tmp);
		//fprintf(fp_out, "gotdata:%s\n", notes_tmp);

		//������
		division = (int)strlen(notes_tmp) / 2;
		//����
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

							if (islog) {
								printf("sec1:%s sec2:%s a:%lf b:%lf type:%d index:%d\n",
									sectmp,
									modarray[modout].section,
									modarray[modout].num / modarray[modout].div,
									(double)i / division, modarray[modout].type,
									modout);
							}
							
							if (modarray[modout].type == 3) {

								//�s�����p�_�ɂ���
								if (tmp[0] != '3')notes_tmp[2 * i] = '5';
							}
							if (modarray[modout].type == 4) {
								//0���ɂ���
								notes_tmp[2 * i + 1] = '0';
							}
						}
						//printf("2\n");
					}
					//printf("1\n");
				}

			}
			else if (type_info_tmp[3] == '1') {
				switch (tmp[0]) {//��ނɉ����đ���Ώۂ��ꎞ�f�[�^�z��Ƃ��ĕۑ�
				case '3':

					//flick
					modarray[modnum].div = division;
					modarray[modnum].num = i;
					modarray[modnum].type = 3;
					modarray[modnum].pos = type_info_tmp[4];
					strcpy(modarray[modnum].section, sectmp);
					//modarray[modnum].section = sectmp;

					if (islog) {
						printf("flick:%d %lf %d %c %s :%d\n", modarray[modnum].div,
							modarray[modnum].num,
							modarray[modnum].type,
							modarray[modnum].pos,
							modarray[modnum].section,
							modnum);
					}

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

					if (islog) {
						printf("damage:%d %lf %d %c %s :%d\n", modarray[modnum].div,
							modarray[modnum].num,
							modarray[modnum].type,
							modarray[modnum].pos,
							modarray[modnum].section,
							modnum);
					}
					
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

	if (ismessage) {
		printf("<info> �t�@�C���̕ϊ����������܂���!\n���̓t�@�C���Ɠ����f�B���N�g����%s���쐬����Ă��邩�m�F���Ă�������\n", output_filename);
		system("PAUSE");
	}
	return 0;
}