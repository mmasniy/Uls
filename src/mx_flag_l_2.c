#include "uls.h"

static char *get_login(uid_t st_uid, bool flag_n);

void mx_take_group_and_size_for_l(t_info *info) {
	struct stat buff;
	struct group *grp;
	char *theOne;
	t_uni_list *tmp2 = info->sub_args;

	for (t_info_l *tmp = info->info_l; tmp; tmp = tmp->next) {
		theOne = mx_strjoin(info->path, tmp2->data);
		lstat(theOne, &buff);
		free(theOne);
		info->total_blocks_l += buff.st_blocks;
		grp = getgrgid(buff.st_gid);
		tmp->nlink = mx_itoa(buff.st_nlink);
		if (grp && !info->flag_n)
			tmp->group_owner = mx_strdup(grp->gr_name);
		else
			tmp->group_owner = mx_itoa(buff.st_gid);
		tmp->login = get_login(buff.st_uid, info->flag_n);
		tmp->size = mx_block_size(tmp, buff);
		tmp2 = tmp2->next;
	}
}

void mx_count_tabs_l(t_info *info) {
	t_tabs_l *tabs_l = info->tabs_l;

	tabs_l->l_nlink = tabs_l->l_login = tabs_l->l_group_owner
		= tabs_l->l_size = tabs_l->l_time_upd = 0;
	for (t_info_l *tmp = info->info_l; tmp; tmp = tmp->next) {
		if (mx_strlen(tmp->nlink) > tabs_l->l_nlink)
			tabs_l->l_nlink = mx_strlen(tmp->nlink);
		if (mx_strlen(tmp->login) > tabs_l->l_login)
			tabs_l->l_login = mx_strlen(tmp->login);
		if (mx_strlen(tmp->group_owner) > tabs_l->l_group_owner)
			tabs_l->l_group_owner = mx_strlen(tmp->group_owner);
		if (!tmp->minor_major && mx_strlen(tmp->size) > tabs_l->l_size)
			tabs_l->l_size = mx_strlen(tmp->size);
		if (mx_strlen(tmp->time_upd) > tabs_l->l_time_upd)
			tabs_l->l_time_upd = mx_strlen(tmp->time_upd);
	}
	if (info->flag_h)
		info->tabs_l->l_size++;
}

void mx_date_time_for_l(t_info *info) {
	struct stat buff;
	t_uni_list *tmp2 = info->sub_args;
	char *theOne;

	for (t_info_l *tmp = info->info_l; tmp; tmp = tmp->next) {
		theOne = mx_strjoin(info->path, tmp2->data);
		lstat(theOne, &buff);
		tmp->time_upd = mx_time_format(info, buff);
		tmp2 = tmp2->next;
		free(theOne);
	}
}

static char *get_login(uid_t st_uid, bool flag_n) {
	char *user = (char *)malloc(256);
	struct passwd *pw = getpwuid(st_uid);
	char *temp;

	if (pw == NULL)
		user = mx_itoa(st_uid);
	else {
		if (flag_n) {
			temp = mx_itoa(pw->pw_uid);
			mx_strcpy(user, temp);
			free(temp);
		}
		else
			mx_strcpy(user, pw->pw_name);
	}
	return user;
}
