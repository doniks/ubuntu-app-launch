/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *     Ted Gould <ted.gould@canonical.com>
 */

#include <gio/gio.h>

G_BEGIN_DECLS

gboolean  app_id_to_triplet      (const gchar *   app_id,
                                  gchar **        package,
                                  gchar **        application,
                                  gchar **        version);
gchar *   desktop_to_exec        (GKeyFile *      desktop_file,
                                  const gchar *   from);
GArray *  desktop_exec_parse     (const gchar *   execline,
                                  const gchar *   uri_list);
GKeyFile * keyfile_for_appid     (const gchar *   appid,
                                  gchar * *       desktopfile);

typedef struct _handshake_t handshake_t;
handshake_t * starting_handshake_start   (const gchar *   app_id,
                                          const gchar *   instance_id,
                                          int timeout_s);
void      starting_handshake_wait        (handshake_t *   handshake);

gboolean   verify_keyfile        (GKeyFile *    inkeyfile,
                                  const gchar * desktop);

G_END_DECLS

