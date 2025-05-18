#include "Sample_wraped_text.h"

VMWCHAR title_name[12] = {};
VMBOOL second = VM_FALSE;
VMCHAR new_data[1201] = {};

void vm_main(void) {

    layer_hdl[0] = -1;
    vm_reg_sysevt_callback(handle_sysevt);
    vm_reg_keyboard_callback(handle_keyevt);
    //vm_font_set_font_size(VM_SMALL_FONT);
    vm_ascii_to_ucs2(title_name, (strlen("Input text:") + 1) * 2, "Input text:");
    vm_input_set_editor_title(title_name);

}

void handle_sysevt(VMINT message, VMINT param) {

    switch (message) {
        case VM_MSG_CREATE:
        case VM_MSG_ACTIVE:
            layer_hdl[0] = vm_graphic_create_layer(0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height(), -1);
            vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
            vm_switch_power_saving_mode(turn_off_mode);
            break;

        case VM_MSG_PAINT:
            vm_switch_power_saving_mode(turn_off_mode);
            vm_input_set_editor_title(title_name);
            if (second == VM_FALSE) {
               vm_input_text3(NULL, 1200, 8, save_text);
               second = VM_TRUE;
            } else {
               show_message(new_data);
            }

            break;

        case VM_MSG_INACTIVE:

            vm_switch_power_saving_mode(turn_on_mode);
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;

        case VM_MSG_QUIT:
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;
    }
}

void handle_keyevt(VMINT event, VMINT keycode) {

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_RIGHT_SOFTKEY) {
        if (layer_hdl[0] != -1) {
            vm_graphic_delete_layer(layer_hdl[0]);
            layer_hdl[0] = -1;
        }
        vm_exit_app();
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_LEFT_SOFTKEY) {
        vm_input_text3(NULL, 1200, 8, save_text);
    }

}

void save_text(VMINT state, VMWSTR text) {

    //VMCHAR new_data[1201];
    VMINT lenght;

    lenght = wstrlen(text);

    if (state == VM_INPUT_OK && lenght > 0) {
       //vm_chset_convert(VM_CHSET_UCS2, VM_CHSET_UTF8, (VMCHAR *)text, new_data, (lenght + 1) * 2); //src, dst
       vm_ucs2_to_ascii(new_data, (lenght + 1) * 2, text);
    } else {
      strcpy(new_data , "");
    }
    show_message(new_data);
}

void show_message(VMSTR text){

	VMWSTR wText = NULL;
	VMINT i = 0;
	VMWCHAR szTemp[61] = {}; //is simboliu sukonstruota USC2 eilute
	VMWCHAR szLine[2] = {};  //vienas USC2 simbolis
	VMINT nStartY = 0;      //einamoji eilutes Y koordinate ekrane
        VMINT text_lengt = 0;
	vm_graphic_color color;

	VMCHAR zZZZ[4] = "WWp"; //Š???
	VMWCHAR cZZZQ[4] = {};
        VMINT sWidth = 0;  //dispaly width
        VMINT sHeight = 0; //display height
        VMINT hZZZ = 0;
        VMINT xBaseline = 0;

        text_lengt = strlen(text);

        sWidth = vm_graphic_get_screen_width();
        sHeight = vm_graphic_get_screen_height();

	color.vm_color_565 = VM_COLOR_WHITE;
	vm_graphic_setcolor(&color);
	vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, sWidth, sHeight);

        if (text_lengt < 1){
           vm_graphic_flush_layer(layer_hdl, 1);
           return;
        }

	color.vm_color_565 = VM_COLOR_BLACK;
	vm_graphic_setcolor(&color);

        vm_ascii_to_ucs2(cZZZQ, (strlen(zZZZ) + 1) * 2, zZZZ);
        xBaseline = vm_graphic_get_string_baseline(cZZZQ);
        hZZZ = vm_graphic_get_string_height(cZZZQ);

        //wText = vm_malloc(sizeof(VMWCHAR) * 512);
        //wText = vm_malloc((text_lengt + 1) * 2);
        //*wText = vm_malloc((text_lengt + 1) * 2);
        //wText = vm_malloc(((text_lengt + 1) * 2) * sizeof(VMWCHAR));
        wText = vm_malloc((text_lengt + 1) * 2);

 	vm_ascii_to_ucs2(wText, (text_lengt + 1) * 2, text); //konvertuojam i USC2

	while(*wText)
	{
                if (nStartY + hZZZ > sHeight){break;}

		szTemp[i++] = *wText; //is simboliu konstruojame USC2 eilute szTemp
		szLine[0] = *wText;
		szLine[1] = 0;

		if( !wstrcmp(szLine, (VMWSTR)(L"\n")) || vm_graphic_get_string_width(szTemp) > sWidth + 1 )
		{			
			if( !wstrcmp(szLine, (VMWSTR)(L"\n")) || !wstrcmp(szLine, (VMWSTR)(L"\r")) || !wstrcmp(szLine, (VMWSTR)(L"\t")) ) {wText++;} //jei simbolis = /n tai ta simboli praleidziam
			szTemp[i-1] = 0;			
	                vm_graphic_textout_to_layer_by_baseline(layer_hdl[0], 0, nStartY, szTemp, wstrlen(szTemp), xBaseline);
			nStartY += hZZZ;
			i = 0;
                        //wmemset(szTemp, 0, sizeof(VMWCHAR)*61);
                        //wmemset(szTemp, L'\0', sizeof(szTemp));
			//memset(szTemp, 0, sizeof(VMWCHAR) * 61);
			memset(szTemp, 0, sizeof(szTemp[0]) * 61);
			//memset(szLine, 0, sizeof(szLine[0]) * 2);

			continue;
		}		
		wText++;
	}
	if(wstrlen(szTemp))
	{
	        vm_graphic_textout_to_layer_by_baseline(layer_hdl[0], 0, nStartY, szTemp, wstrlen(szTemp), xBaseline);

	}

        vm_graphic_flush_layer(layer_hdl, 1);

	//vm_free(wText); //?????????????????????????????????????
}