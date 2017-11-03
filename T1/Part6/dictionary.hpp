#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#define GET_OBJ(builder, name) GTK_WIDGET(gtk_builder_get_object(builder, name))
#define SPIN_GET_VALUE(BUILDER, ID) gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(BUILDER, ID))))
#define ENTRY_GET_TEXT(BUILDER, ID) gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(BUILDER, ID))))

#define ADD_OBJECT_DIALOG "dialog3"
#define ADD_POINT_DIALOG "dialog1"
#define ADD_STRAIGHT_DIALOG "dialog2"
#define ADD_POLYGON_NAME_DIALOG "dialog5"
#define ADD_POLYGON_COORDINATE_DIALOG "dialog6"
#define REMOVE_OBJECT_DIALOG "dialog4"
#define EMPTY_DF_DIALOG "dialog7"
#define TRANSLATE_OBJECT_DIALOG "dialogTranslate"
#define SCALE_OBJECT_DIALOG "dialogScale"
#define ROTATE_OBJECT_DIALOG "dialogRotate"

#define WINDOW_STEP_SPIN "spinbutton1"
#define POINT_X_SPIN "spinbutton6"
#define POINT_Y_SPIN "spinbutton7"
#define STRAIGHT_A_X_SPIN "spinbutton8"
#define STRAIGHT_A_Y_SPIN "spinbutton9"
#define STRAIGHT_B_X_SPIN "spinbutton10"
#define STRAIGHT_B_Y_SPIN "spinbutton11"
#define POLYGON_X_SPIN "spinbutton12"
#define POLYGON_Y_SPIN "spinbutton13"
#define REMOVE_ID_SPIN "spinbutton14"
#define ROTATE_ID_SPIN "spinIDRotate"
#define ROTATE_DEGREES_SPIN "spinDegreesRotate"
#define ROTATE_X_SPIN "spinXRotate"
#define ROTATE_Y_SPIN "spinYRotate"
#define TRANSLATE_ID_SPIN "spinIDTranslate"
#define TRANSLATE_X_SPIN "spinDXTranslate"
#define TRANSLATE_Y_SPIN "spinDYTranslate"
#define SCALE_ID_SPIN "spinIDScale"
#define SCALE_STEP_SPIN "spinStepScale"

#define POINT_NAME_ENTRY "entry2"
#define STRAIGHT_NAME_ENTRY "entry3"
#define POLYGON_NAME_ENTRY "entry5"

#endif
