/*
 * help-cmd.c -- Provide help
 *
 * ====================================================================
 * Copyright (c) 2000 CollabNet.  All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at http://subversion.tigris.org/license-1.html.
 * If newer versions of this license are posted there, you may use a
 * newer version instead, at your option.
 * ====================================================================
 */

/* ==================================================================== */



/*** Includes. ***/

#include "svn_client.h"
#include "svn_string.h"
#include "svn_error.h"
#include "cl.h"


/*** Code. ***/

/* Print the canonical command name for CMD, all its aliases,
   and if HELP is set, print the help string for the command too. */
static void
print_command_info (const svn_cl__cmd_desc_t *cmd_desc,
                    svn_boolean_t help, 
                    apr_pool_t *pool)
{
  const svn_cl__cmd_desc_t *this_cmd
    = svn_cl__get_canonical_command (cmd_desc->name);
  const svn_cl__cmd_desc_t *canonical_cmd = this_cmd;
  svn_boolean_t first_time;

  /* Print the canonical command name. */
  fputs (canonical_cmd->name, stdout);

  /* Print the list of aliases. */
  first_time = TRUE;
  for (this_cmd++; (this_cmd->name && this_cmd->is_alias); this_cmd++) 
    {
      if (first_time) {
        printf (" (");
        first_time = FALSE;
      }
      else
        printf (", ");
      
      printf ("%s", this_cmd->name);
    }

  if (! first_time)
    printf (")");
  
  if (help)
    printf (": %s\n", canonical_cmd->help);
}


/* Print a generic (non-command-specific) usage message. */
static void
print_generic_help (apr_pool_t *pool)
{
  static const char usage[] =
    "usage: svn <subcommand> [options] [args]\n"
    "Type \"svn help <subcommand>\" for help on a specific subcommand.\n"
    "\n"
    "Most subcommands take file and/or directory arguments, recursing\n"
    "on the directories.  If no arguments are supplied to such a\n"
    "command, it will recurse on the current directory (inclusive) by\n" 
    "default.\n"
    "\n"
    "Available subcommands:\n";
  int i = 0;

  printf ("%s", usage);
  while (svn_cl__cmd_table[i].name) 
    {
      /*  for (i = 0; i < max; i++) */
      if (! svn_cl__cmd_table[i].is_alias)
        {
          printf ("   ");
          print_command_info (svn_cl__cmd_table + i, FALSE, pool);
          printf ("\n");
        }
      i++;
    }
}


/* Print either generic help, or command-specific help for each
 * command in ARGV.  OPT_STATE is unused and may be null.
 * 
 * Unlike all the other command routines, ``help'' has its own
 * option processing.  Of course, it does not accept any options :-),
 * just command line args.
 */
svn_error_t *
svn_cl__help (svn_cl__opt_state_t *opt_state,
              apr_array_header_t *targets,
              apr_pool_t *pool)
{
  int i;

  if (targets->nelts)
    for (i = 0; i < targets->nelts; i++)
      {
        svn_string_t *this = (((svn_string_t **) (targets)->elts))[i];
        const svn_cl__cmd_desc_t *cmd
          = svn_cl__get_canonical_command (this->data);
        if (cmd)
          print_command_info (cmd, TRUE, pool);
        else
          fprintf (stderr, "\"%s\": unknown command.\n\n", this->data);
      }
  else
    print_generic_help (pool);

  return SVN_NO_ERROR;
}



/* 
 * local variables:
 * eval: (load-file "../svn-dev.el")
 * end: 
 */
