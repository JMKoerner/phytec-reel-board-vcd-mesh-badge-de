/*
 * Copyright (c) 2019 PHYTEC Messtechnik GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <sys/util.h>
#include <stdio.h>

#include <drivers/display.h>
#include <display/cfb.h>
#include <lvgl.h>

#include "qrcodegen.h"
#include "vcd.h"

#define CANVAS_HEIGHT 300
#define CANVAS_WIDTH  300
#define CANVAS_H 20
#define CANVAS_W 250
#define SCALE_QR 2
#define SCALE_SM 2


static void lv_canvas_for_Qr(const char *qr_text){
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX*SCALE_QR];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX*SCALE_QR];
  	bool ok = qrcodegen_encodeText(qr_text, tempBuffer, qrcode, errCorLvl,qrcodegen_VERSION_MIN,
	  qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok){
		printf ("Generate QR Code\n");
		int size = qrcodegen_getSize(qrcode);
		printf("OriginalSize: %d, QRSizeNeu: %d\n", size, size*SCALE_QR);

		static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(150, 150)];
		lv_obj_t * canvas = lv_canvas_create(lv_scr_act(), NULL);
		lv_canvas_set_buffer(canvas, cbuf, size*SCALE_QR, size*SCALE_QR, LV_IMG_CF_TRUE_COLOR);
		lv_obj_align(canvas, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
		lv_canvas_fill_bg(canvas, LV_COLOR_BLACK);

		int border = 0;
		int count = 0;
//		printf("White Pixel: X    Y\n");
		for (int y = -border; y < size + border; y++){
			for(int x = -border; x < size + border; x++){
				bool check_color = qrcodegen_getModule(qrcode, x , y);
					if (check_color == false){		// if pixel white
						/*
						if(x>9)
							printf("             %d   %d\n", x, y);
						else
							printf("             %d    %d\n", x, y);
						*/
						for (int i =0; i < SCALE_QR; i++){
							for (int j = 0; j < SCALE_QR; j++){
								lv_canvas_set_px(canvas, x*SCALE_QR+i, y*SCALE_QR+j, LV_COLOR_WHITE);
					  		}
				  		}
			  		}

	  		}
			count ++;
  		}
		printf("Number of QR-Pixel: %d\n", count);
	}
	else
		printf("QR-Fehler!");
}

static void lv_text_bottom (const char *text_arr, const char *text_arr2){

	static lv_style_t style;
	lv_style_copy(&style, &lv_style_plain);
	style.text.color = LV_COLOR_BLACK;
	style.text.font = &lv_font_roboto_28;

	static lv_style_t style2;
	lv_style_copy(&style2, &lv_style_plain);
	style2.text.color = LV_COLOR_WHITE;
	style2.text.font = &lv_font_roboto_16;

	static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(250, 50)];
	lv_obj_t *canvas_bottom = lv_canvas_create(lv_scr_act(), NULL);
	lv_canvas_set_buffer(canvas_bottom, cbuf, 250, 30, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(canvas_bottom, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, -30);
	lv_canvas_fill_bg(canvas_bottom, LV_COLOR_WHITE);
	lv_canvas_draw_text(canvas_bottom, -20, 0, 300, &style, text_arr, LV_LABEL_ALIGN_CENTER);

	static lv_color_t cbuf2[LV_CANVAS_BUF_SIZE_TRUE_COLOR(250, 50)];
	lv_obj_t *canvas_bottom2 = lv_canvas_create(lv_scr_act(), NULL);
	lv_canvas_set_buffer(canvas_bottom2, cbuf2, 250, 25, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(canvas_bottom2, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_canvas_fill_bg(canvas_bottom2, LV_COLOR_BLACK);
	lv_canvas_draw_text(canvas_bottom2, -20, 3, 300, &style2, text_arr2, LV_LABEL_ALIGN_CENTER);
}

static void lv_text_right(const char *text_right, const char *text_right2){
	static lv_style_t style;
	lv_style_copy(&style, &lv_style_plain);
	style.text.color = LV_COLOR_BLACK;
	style.text.font = &lv_font_roboto_28;

	static lv_style_t style2;
	lv_style_copy(&style2, &lv_style_plain);
	style2.text.color = LV_COLOR_BLACK;
	style2.text.font = &lv_font_roboto_16;

	static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(300, 50)];
	lv_obj_t *canvas_right1= lv_canvas_create(lv_scr_act(), NULL);
	lv_canvas_set_buffer(canvas_right1, cbuf, 182, 30, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(canvas_right1, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
	lv_canvas_fill_bg(canvas_right1, LV_COLOR_WHITE);
	lv_canvas_draw_text(canvas_right1, 0, 0, 300, &style, text_right, LV_LABEL_ALIGN_LEFT);

	static lv_color_t cbuf2[LV_CANVAS_BUF_SIZE_TRUE_COLOR(300, 80)];
	lv_obj_t *canvas_right2= lv_canvas_create(lv_scr_act(), NULL);
	lv_canvas_set_buffer(canvas_right2, cbuf2, 180, 20, LV_IMG_CF_TRUE_COLOR);
	lv_obj_align(canvas_right2, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 35);
	lv_canvas_fill_bg(canvas_right2, LV_COLOR_WHITE);
	lv_canvas_draw_text(canvas_right2, 0, 0, 300, &style2, text_right2, LV_LABEL_ALIGN_LEFT);
}

void vcd_epaper(const char *text_bottom, const char *vcdProf, bool scr_new)
{
	if (scr_new) {
		lv_obj_t *default_screen = lv_scr_act();
		lv_obj_clean(default_screen);

		lv_canvas_for_Qr(qr_text);

		lv_text_bottom(text_bottom, text_bottom2);
		lv_text_right(text_right, vcdProf);
	}
	lv_scr_load(lv_scr_act());

	lv_task_handler();
	printk("lv_task_handler()\n");
}
