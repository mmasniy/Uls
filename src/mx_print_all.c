#include "uls.h"
// Errors
void mx_invalid_usage() {
	mx_printerr(INVALID_USAGE);
	mx_printerr("\n");
}

void mx_arg_not_exist(t_info *info) {
	for (int i = 0; i < info->argc; i++)
		if (!info->where_what[i]) {
			mx_printerr("uls: ");
			mx_printerr(info->argv[i]);
			mx_printerr(": No such file or directory");
			mx_printerr("\n");
		}
}
// Print args
static void print_tabs(int n) {
	for (int i = 0; i < n; i++)
		write(1, " ", 1);
}

static void basic_print(t_info *info) {
	int num_of_lines = mx_num_of_cols(info);
	int j;
	int sub_r;
	int tab_len = 0;

	for (int i = 0; i < num_of_lines; i++) {
		j = 0;
		sub_r = 0;
		for (t_uni_list *tmp = info->sub_args; tmp; tmp = tmp->next, j++) {
			if ((j + num_of_lines - i) % num_of_lines == 0) {
				mx_printstr(tmp->data);
				if (sub_r + num_of_lines < info->num_of_sub) {
					tab_len = info->max_sub_len - mx_strlen(tmp->data);
					print_tabs(tab_len + (8 - (info->max_sub_len % 8)));
				}
			}
			++sub_r;
		}
		mx_printchar('\n');
	}
}

static void count_tabs_l(t_info *info) {
	t_tabs_l *tabs_l = (t_tabs_l *)malloc(sizeof(t_tabs_l));
	tabs_l->l_nlink = tabs_l->l_login = tabs_l->l_group_owner
	= tabs_l->l_sym_num = tabs_l->l_time_upd = 0;

	for (t_info_l *tmp = info->info_l; tmp; tmp = tmp->next) {
		if (mx_strlen(tmp->nlink) > tabs_l->l_nlink)
			tabs_l->l_nlink = mx_strlen(tmp->nlink);
		if (mx_strlen(tmp->login) > tabs_l->l_login)
			tabs_l->l_login = mx_strlen(tmp->login);
		if (mx_strlen(tmp->group_owner) > tabs_l->l_group_owner)
			tabs_l->l_group_owner = mx_strlen(tmp->group_owner);
		if (mx_strlen(tmp->sym_num) > tabs_l->l_sym_num)
			tabs_l->l_sym_num = mx_strlen(tmp->sym_num);
		if (mx_strlen(tmp->time_upd) > tabs_l->l_time_upd)
			tabs_l->l_time_upd = mx_strlen(tmp->time_upd);
	}
	info->tabs_l = tabs_l;
}

static void print_l(t_info *info) {
	t_uni_list *t2 = info->sub_args;
	t_tabs_l *tmp3 = info->tabs_l;

	for (t_info_l *tmp = info->info_l; tmp; tmp = tmp->next, t2 = t2->next) {
		mx_printstr(tmp->access);
		print_tabs(tmp3->l_nlink - mx_strlen(tmp->nlink));
		mx_printstr(tmp->nlink);
		print_tabs(1);
		mx_printstr(tmp->login);
		print_tabs(tmp3->l_login - mx_strlen(tmp->login) + 2);
		mx_printstr(tmp->group_owner);
		print_tabs(tmp3->l_group_owner - mx_strlen(tmp->group_owner));
		print_tabs(tmp3->l_sym_num - mx_strlen(tmp->sym_num) + 2);
		mx_printstr(tmp->sym_num);
		print_tabs(1);
		mx_printstr(tmp->time_upd);
		print_tabs(tmp3->l_time_upd - mx_strlen(tmp->time_upd) + 1);
		mx_printstr(t2->data);
		mx_printchar('\n');
	}
}

static void print_1(t_info *info) {
	mx_printstr("Path = ");
	mx_printstr(info->sub_args->path);
	mx_printchar('\n');
	for (t_uni_list *tmp = info->sub_args; tmp; tmp = tmp->next) {
		mx_printstr(tmp->data);
		mx_printchar('\n');
	}
	mx_printchar('\n');
}

void mx_print_arg(t_info *info, bool folder) {
	int f = 1;

	if (info->first_argv)
		info->first_argv = 0;
	else
		mx_printchar('\n');

	if (folder) {
		mx_printstr(mx_up_to_one(info->sub_args->path));
		mx_printstr(":\n");
	}
	if (info->all_our_flags) {
		for (int i = mx_strlen(info->all_our_flags) - 1; i >= 0; i--) {
			if (info->all_our_flags[i] == 'l' && f--) {
				count_tabs_l(info);
				mx_printstr("total ");
				mx_printint(info->total_blocks_l);
				mx_printchar('\n');
				print_l(info);
				info->total_blocks_l = 0;
				break;
			}
			else if (info->all_our_flags[i] == '1' && f--) {
				print_1(info);
				break;
			}
		}
	}
	if (f)
		basic_print(info);
}
