
#include "./linc_dialogs.h"

#include <gtk/gtk.h>


namespace linc {

    namespace dialogs {

      static bool gtk_inited = false;

      ::String open_gtk_dialog(GtkFileChooserAction action, ::String title, ::Array<Dynamic> filters)
      {

        if(!gtk_inited) {
            gtk_init(NULL, NULL);
            gtk_inited = true;
        }

        ::String result;

        GtkWidget *dialog;

        dialog = gtk_file_chooser_dialog_new(
            title.c_str(),
            NULL,       //parent window
            action,
            "Cancel", GTK_RESPONSE_CANCEL,
            "Select", GTK_RESPONSE_ACCEPT,
            NULL 
        );

        if(action != GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER && (filters!=null()) && filters->size() > 0) {

          // loop through the given filters adding them
            int size = filters->size();
            for(int i=0; i<size; ++i) {
             
              GtkFileFilter *a_filter = gtk_file_filter_new();
              lincDialogsFilter filter(filters[i]);

              gtk_file_filter_add_pattern(a_filter, filter.pattern.c_str());
              gtk_file_filter_set_name(a_filter, filter.display.c_str());
             
              //add to the dialog
              gtk_file_chooser_add_filter( GTK_FILE_CHOOSER( dialog ), a_filter );
             
            } //each filter

        } //not folder select && has filters

        if( gtk_dialog_run( GTK_DIALOG(dialog) ) == GTK_RESPONSE_ACCEPT ) {
            char *filename;
            filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER( dialog ) );
            result = ::String( filename );
            g_free( filename );
        }

        gtk_widget_destroy( dialog );

            //poll until events are done
        while( gtk_events_pending() ) gtk_main_iteration();

            //return the path or ""
        return result;

      } //open_gtk_dialog

      ::String open(::String title, ::Array<Dynamic> filters)
      {
        return open_gtk_dialog(GTK_FILE_CHOOSER_ACTION_OPEN, title, filters);
      }

      ::String save(::String title, Dynamic filter)
      {
				cpp::ArrayBase filters = Dynamic( Array_obj<Dynamic>::__new() );
				if (filter != null()) {
          filters->__Field(HX_CSTRING("push"), hx::paccDynamic )(filter);
        }

        return open_gtk_dialog(GTK_FILE_CHOOSER_ACTION_SAVE, title, filters);
      }
      ::String folder(::String title) {
        return open_gtk_dialog(GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, title, 0);
      }

    } //dialogs namespace

} //linc namespace
