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
int isauto_suspatch_exec = 0;
char suspatch_path[500];

const char channelsign[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
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

char type_info_tmp2[6 + 1];//fuzzy
char notes_tmp2[1920*2 * 2 + 1];//fuzzy,slide

char type_info_tmp3[6 + 1];//fuzzy
char notes_tmp3[1920*2 * 2 + 1];//fuzzy,�n�_�I�_�폜�p

char tmp[2 + 1];
int division;
char sectmp[3 + 1];
char sectmp2[3 + 1];

int fuzzynum = 0;
int fuzzyloop = 0;
int sec;
int isauto_collectnum = 0;

int isfirstslide = 1;
int isfirstnote = 1;

int k;

slidemod modarray[50000 + 1];
int modnum = 0, modout = 0;

void clrbuf(void) {
	for (int i = 0; i < 1920 * 2 + 1 + 8; i++) {
		buffer1[i] = 0;
		buffer2[i] = 0;
		buffer3[i] = 0;
		buffer4[i] = 0;
		buffer5[i] = 0;
	}
	for (int i = 0; i < 1920 * 2; i++) {
		notes_tmp2[i] = '0';
		notes_tmp3[i] = '0';
	}
	notes_tmp2[0] = 0; 
	notes_tmp3[0] = 0;
}

int main(int argc, char* argv[]) {
	//ini�ǂݍ���
	if ((fp = fopen("fuzzy_setting.ini", "r")) == NULL) {
		//�t�@�C�������݂��Ȃ��̂ŐV���ɍ��
		if ((fp = fopen("fuzzy_setting.ini", "w")) == NULL) {
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
				strcpy(output_filename, "output.sus");
			}
			if (argc >= 4) {
				strcpy(suspatch_path, argv[3]);
			}
			else {
				strcpy(suspatch_path, "suspatcher.exe");
			}
			fprintf(fp, "[input]\n");
			fprintf(fp, "isauto input = %d\n", isauto_input);
			fprintf(fp, "input filename = %s\n", input_filename);
			fprintf(fp, "\n[output]\n");
			fprintf(fp, "output filename = %s\n", output_filename);
			fprintf(fp, "\n[other_settings]\n");
			fprintf(fp, "ismessage = %d\n", ismessage);
			fprintf(fp, "isauto execute suspatcher = %d\n", isauto_suspatch_exec);
			fprintf(fp, "sus patcher filename = %s\n", suspatch_path);
			fprintf(fp, "isauto collectnum = %d\n", isauto_collectnum);
		}
	}
	else {
		//�t�@�C�����݂��Ă���̂Ńf�[�^�ǂ�
		fscanf(fp, "%[^\n]%c", buffer1,&buf);

		fscanf(fp, "%[^\n]%c", buffer1,&buf);
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
		sscanf(buffer1, "%[^0-9]%d", buffer2, &isauto_suspatch_exec);

		fscanf(fp, "%[^\n]%c", buffer1, &buf);
		sscanf(buffer1, "%s %s %s %s %s", buffer2, buffer3, buffer4,buffer5, suspatch_path);

		fscanf(fp, "%[^\n]%c", buffer1, &buf);
		sscanf(buffer1, "%[^0-9]%d", buffer2, &isauto_collectnum);

	}
	fclose(fp);
	//�����܂�

	//start of input part
	if (!isauto_input) {
		if (argc == 1) {
			printf("<info> sus�t�@�C��������exe�Ƀh���b�O�A���h�h���b�v���Ă�������\n");
			system("PAUSE");
			exit(0);
		}
		strcpy(input_filename, argv[1]);
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

	if (!isauto_collectnum) {
		printf("�X���C�h�̍ő哯��������(1�ȏ�̐����l)����͂��Ă�������>>>");
		scanf("%d", &fuzzynum);
	}

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

			if (isauto_collectnum) {
				k = 0;
				while (type_info_tmp[5] != channelsign[k]) {
					k++;
				}

				k++;
				if (k > fuzzynum) {
					isfirstslide = 1;
					isfirstnote = 1;
				}

				if (isfirstslide == 1) {
					//�����Ɏn�_(�폜)����������
					for (int i = fuzzynum; i < k; i++) {
						sprintf(buffer1, "#00030%c:11", channelsign[i]);
						fprintf(fp_out, "%s\n", buffer1);
						clrbuf();
					}
					isfirstslide = 0;
				}
				if (isfirstnote == 1) {
					//�n�_�폜�p
					for (int i = fuzzynum; i < k; i++) {
						sprintf(buffer2, "#00010:31");
						fprintf(fp_out, "%s\n", buffer2);
						buffer2[0] = 0;
						sprintf(buffer2, "#00010:41");
						fprintf(fp_out, "%s\n", buffer2);
						buffer2[0] = 0;
					}
					isfirstnote = 0;
				}
				fuzzynum = k;
			}
			else if (isfirstslide) {
				
				//�����Ɏn�_(�폜)����������
				for (int i = 0; i < fuzzynum+1; i++) {
					sprintf(buffer1, "#00030%c:11", channelsign[i]);
					fprintf(fp_out, "%s\n", buffer1);
					clrbuf();
				}
				//�n�_�폜�p
				for (int i = 0; i < fuzzynum+1; i++) {
					sprintf(buffer2, "#00010:31");
					fprintf(fp_out, "%s\n", buffer2);
					buffer2[0] = 0;
					sprintf(buffer2, "#00010:41");
					fprintf(fp_out, "%s\n", buffer2);
					buffer2[0] = 0;
				}

				isfirstslide = 0;
			}
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

		if (type_info_tmp[3] == '3') {
			for (int i = 0; i < division; i++) {
				tmp[0] = notes_tmp[2 * i];//type
				tmp[1] = notes_tmp[2 * i + 1];//size

				if (tmp[0] == '1' || tmp[0] == '2') {//�n�_�I�_�Ȃ�

					//�����Ɏn�[�I�[������
					if (tmp[0] == '1') {
						//�n�_
						if (i == 0) {
							sscanf(sectmp, "%3d", &sec);
							sec--;
							sprintf(sectmp2, "%03d", sec);

							type_info_tmp2[0] = 0;
							sprintf(type_info_tmp2, "%03s30%c", sectmp2, type_info_tmp[5]);
							type_info_tmp2[6] = 0;
							type_info_tmp3[0] = 0;
							sprintf(type_info_tmp3, "%03s10", sectmp2);
							type_info_tmp3[5] = 0;

							for (int j = 0; j < 1920; j++) {
								if (j == 1920 - 1) {
									notes_tmp2[2 * j] = '5';
									notes_tmp2[2 * j + 1] = '1';
									notes_tmp3[2 * j] = '4';
									notes_tmp3[2 * j + 1] = '1';
								}
								else {
									notes_tmp2[2 * j] = '0';
									notes_tmp2[2 * j + 1] = '0';
									notes_tmp3[2 * j] = '0';
									notes_tmp3[2 * j + 1] = '0';
								}
							}
							notes_tmp2[2 * 1920] = 0; notes_tmp3[2 * 1920] = 0;
							sprintf(buffer4, "#%s:%s", type_info_tmp2, notes_tmp2);
							sprintf(buffer5, "#%s:%s", type_info_tmp3, notes_tmp3);
						}
						else {
							type_info_tmp2[0] = 0;
							sprintf(type_info_tmp2, "%03s30%c", sectmp, type_info_tmp[5]);
							type_info_tmp2[6] = 0;
							type_info_tmp3[0] = 0;
							sprintf(type_info_tmp3, "%03s10", sectmp);
							type_info_tmp3[5] = 0;

							for (int j = 0; j < 1920; j++) {
								if (j == (i * (1920 / division)-1)) {
									notes_tmp2[2 * j] = '5';
									notes_tmp2[2 * j + 1] = '1';
									notes_tmp3[2 * j] = '4';
									notes_tmp3[2 * j + 1] = '1';
								}
								else {
									notes_tmp2[2 * j] = '0';
									notes_tmp2[2 * j + 1] = '0';
									notes_tmp3[2 * j] = '0';
									notes_tmp3[2 * j + 1] = '0';
								}
							}
							notes_tmp2[2 * 1920] = 0; notes_tmp3[2 * 1920] = 0;
							sprintf(buffer4, "#%s:%s", type_info_tmp2, notes_tmp2);
							sprintf(buffer5, "#%s:%s", type_info_tmp3, notes_tmp3);
						}
					}
					else if (tmp[0] == '2') {
						//�I�_
						if (i == 1920 - 1) {
							//���Ԃ�Ȃ�����ȗ�()
						}
						else {
							type_info_tmp2[0] = 0;
							sprintf(type_info_tmp2, "%03s30%c", sectmp, type_info_tmp[5]);
							type_info_tmp2[6] = 0;
							type_info_tmp3[0] = 0;
							sprintf(type_info_tmp3, "%03s10", sectmp);
							type_info_tmp3[5] = 0;

							for (int j = 0; j < 1920; j++) {
								if (j == (i * (1920 / division) + 1)) {
									notes_tmp2[2 * j] = '5';
									notes_tmp2[2 * j + 1] = '1';
									notes_tmp3[2 * j] = '4';
									notes_tmp3[2 * j + 1] = '1';
								}
								else {
									notes_tmp2[2 * j] = '0';
									notes_tmp2[2 * j + 1] = '0';
									notes_tmp3[2 * j] = '0';
									notes_tmp3[2 * j + 1] = '0';
								}
							}
							notes_tmp2[2 * 1920] = 0; notes_tmp3[2 * 1920] = 0;
							sprintf(buffer2, "#%s:%s", type_info_tmp2, notes_tmp2);
							sprintf(buffer3, "#%s:%s", type_info_tmp3, notes_tmp3);
						}
					}
					notes_tmp[2 * i] = '5';//�s�����p�_��
					notes_tmp[2 * i + 1] = tmp[1];
				}
				else {
					notes_tmp[2 * i] = tmp[0];
					notes_tmp[2 * i + 1] = tmp[1];
				}
			}
		}
		strcat(buffer1, "#");
		strcat(buffer1, type_info_tmp);
		strcat(buffer1, ":");
		strcat(buffer1, notes_tmp);
		fprintf(fp_out, "%s\n", buffer1);
		fgetc(fp);

		if (buffer2[0] != 0) {
			fprintf(fp_out, "%s\n", buffer2);
		}
		if (buffer3[0] != 0) {
			fprintf(fp_out, "%s\n", buffer3);
		}
		if (buffer4[0] != 0) {
			fprintf(fp_out, "%s\n", buffer4);
		}
		if (buffer5[0] != 0) {
			fprintf(fp_out, "%s\n", buffer5);
		}
		
		clrbuf();
	}
	sscanf(sectmp, "%3d", &sec);
	sec++;
	sprintf(sectmp, "%03d", sec);
	//�����ɏI�_��
	fuzzynum++;
	for (int i = 0; i < fuzzynum; i++) {
		sprintf(buffer1, "#%3s30%c:21", sectmp, channelsign[i]);
		fprintf(fp_out, "%s\n", buffer1);
		clrbuf();
	}

	fclose(fp); fclose(fp_out);

	if (ismessage) {
		
		printf("<info> �t�@�C���̕ϊ����������܂���!\n���̓t�@�C���Ɠ����f�B���N�g����%s���쐬����Ă��邩�m�F���Ă�������\n", output_filename);
		if (isauto_suspatch_exec) {
			sprintf(buffer1, "start %s %s", suspatch_path, output_filename);
			printf("���s�R�}���h:%s\n", buffer1);

			printf("<info>������suspatcher.exe�����s���܂��B(���̃\�t�g�E�F�A�͏I������܂�)\n");
			system("PAUSE");
			system(buffer1);
		}
		else {
			printf("<info>�\�t�g�E�F�A���I�����܂�\n");
			system("PAUSE");
		}
	}
	else {
		if (isauto_suspatch_exec) {
			sprintf(buffer1, "start %s %s", suspatch_path, output_filename);
			system(buffer1);
		}
	}
	return 0;
}