#include "uls.h"

static void info_start_2(t_info *info);

t_info *mx_info_start(int argc, char **argv) {
	t_info *info = (t_info *)malloc(sizeof(t_info));

	info->argc = argc;
	info->argv = argv;
	info->num_of_arg = 0;
	info->first_argv = 1;
	info->flags_exist = 0;
	info->file_exist = 0;
	info->folder_exist = 0;
	info->where_what = (int *)malloc(sizeof(int) * argc);
	info->sub_args = NULL;
	info->path = NULL;
	info->info_l = NULL;
	info->tabs_l = NULL;
	info->num_of_sub = 0;
	info_start_2(info);
	for (int i = 0; i < argc; i++)
		info->where_what[i] = 0;
	return info;
}

static void info_start_2(t_info *info) {
	info->max_sub_len = 0;
	info->total_blocks_l = 0;
	info->print_flag = '0';
	info->sort_flag = '0';
	info->p_F_flag = '0';
	info->flag_a = 0;
	info->flag_A = 0;
	info->flag_R = 0;
	info->flag_G = 0;
	info->flag_h = 0;
	info->flag_dog = 0;
	info->flag_e = 0;
	info->flag_T = 0;
	info->flag_C = 0;
	info->flag_r = 0;
	info->flag_t = 0;
	info->flag_f = 0;
	info->flag_g = 0;
	info->flag_n = 0;
	info->flag_d = 0;
}
