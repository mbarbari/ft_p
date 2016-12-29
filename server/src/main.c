/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barbare <barbare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/04 09:55:16 by barbare           #+#    #+#             */
/*   Updated: 2016/12/28 10:38:34 by barbare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <libft.h>
//#ifdef _XOPEN_SOURCE
    //#include <unistd.h>
//#elif
    #include <crypt.h>
//#endif
#include "config.h"
#include "options.h"
#include "server.h"
#include "error.h"
#include <stdio.h> //TODO: a virer
#include <sys/stat.h>
#include <fcntl.h>

#define ROOTACCOUNT "root:"
#define ROOTPATH ":./root"

static int          usage(void)
{
     printf(MESS_USAGE);
     return (ERROR);
}

static t_serv set_config(t_serv config, t_options options)
{
    int file_auth;

    if ((options.opts & OPT_B) &&
            ft_isdigit(options.parameters[OPT_B][0]))
        config.backlog = ft_atoi(options.parameters[OPT_B]);
    else
        config.backlog = 5;
    if ((options.opts & OPT_P))
        config.authorized = options.parameters[OPT_P];
    else
        config.authorized = "/authorized";
    if (access(config.authorized, 0 | F_OK) != 0)
    {
        file_auth = open(config.authorized, O_WRONLY | O_CREAT, 0700);
        dprintf(file_auth, "%s%s%s", ROOTACCOUNT,
                crypt("root", SALT), ROOTPATH);
        close(file_auth);
    }
    return (config);
}

int                 main(int ac, const char **av, const char **envp)
{
    t_serv    config;
    t_env           env;

    (void)env;
    if (ac < 2)
        return (usage());
    if (ft_isdigit(av[ac - 1][0]))
        config.port = ft_atoi(av[ac - 1]);
    else
        return (usage());
    config = set_config(config, set_short_options(&av[1], ac - 1));
    env.env = (char **)envp;
    server(config, env);
    return (0);
}