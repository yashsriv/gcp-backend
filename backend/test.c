#include "gcp.h"
#include <stdio.h>
// #include <string.h>

int
main (int argc, char **argv)
{
  GCPObject *gcp = gcp_object_new ();
  GError *error = NULL;
  gchar *access_token = g_malloc (150);
  gint expires_in;
  gboolean res = get_access_token (&access_token, &expires_in, &error);
  g_assert (res == TRUE);
  // g_print ("%ld\n", strlen (access_token));
  // if (get_access_token (&access_token, &expires_in, &error))
  //   g_print ("%s\n", access_token);
  // else
  //   g_print ("get_access_token() failed!\n");
  // GList *printers = gcp_object_get_printers (gcp, access_token);

  GHashTableIter iter;
  gpointer key, value;

  // give "ALL" as third param to get a list of all printers intead of only active printers
  // give NULL as third param to get a list of only active printers
  GHashTable *printers = (GHashTable *)gcp_object_get_printers (gcp, access_token, "ALL");


  // while(printers != NULL)
  // {
    // g_print ("%s\n", (gchar *)printers->data);
    // printers = printers->next;
  // }


  g_hash_table_iter_init (&iter, printers);
  while (g_hash_table_iter_next (&iter, &key, &value))
  {
      g_print ("%s => %s\n\n", (gchar *)key, (gchar *)value);
  }

  gchar *uid = g_malloc (50);
  g_print ("Enter uid of printer to get printer options:\n");
  scanf ("%s", uid);
  // g_print ("%s\n", uid);
  // const gchar *uid = "__google__docs";
  GHashTable *printer_options = (GHashTable *)gcp_object_get_printer_options (gcp, uid, access_token);
  g_assert (printer_options != NULL);
  GList *keys = g_hash_table_get_keys (printer_options);
  while (keys != NULL)
  {
    if(g_strcmp0 ((gchar *)keys->data, "vendor_capability_list") == 0)
    {
      g_print ("vendor_capability_list\n");
      GList *vendor_capability_list = (GList *)g_hash_table_lookup (printer_options, keys->data);
      g_assert (vendor_capability_list != NULL);
      while (vendor_capability_list != NULL)
      {
        vendor_capability *capabilities = vendor_capability_list->data;
        g_print ("%s\n", capabilities->id);
        g_print ("%s\n", capabilities->display_name);
        g_print ("%s\n", capabilities->type);

        GList *vc_options = capabilities->options;
        while(vc_options != NULL)
        {
          vendor_capability_option *vc_option = vc_options->data;
          g_print ("%s\n", vc_option->display_name);
          g_print ("%d\n", vc_option->is_default);
          g_print ("%s\n", vc_option->value);
          vc_options = vc_options->next;
        }

        vendor_capability_list = vendor_capability_list->next;
      }
    }
    else if (g_strcmp0 ((gchar *)keys->data, "media_size_options_list") == 0)
    {
      g_print ("media_size_options_list\n");
      GList *media_size_options_list = (GList *)g_hash_table_lookup (printer_options, keys->data);
      g_assert (media_size_options_list != NULL);
      while (media_size_options_list != NULL)
      {
        media_size *option = media_size_options_list->data;
        g_print ("%s\n", option->name);
        g_print ("%s\n", option->vendor_id);
        g_print ("%s\n", option->custom_display_name);
        g_print ("%d\n", option->height_microns);
        g_print ("%d\n", option->width_microns);
        g_print ("%d\n", option->is_default);
        g_print ("%d\n", option->is_continuous_feed);

        media_size_options_list = media_size_options_list->next;
      }
    }
    keys = keys->next;
  }

  // g_print ("%s\n", printer_options);

  g_object_unref (gcp);
  return 0;
}
