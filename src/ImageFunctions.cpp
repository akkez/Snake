#include "Snake.h"

Color currentColor;
void* FONT = GLUT_BITMAP_9_BY_15;
void* ImageCopyBuffer = NULL;

Color getColor(int red, int green, int blue) {
	Color x;
	x.red = red;
	x.green = green;
	x.blue = blue;
	return x;
}

void ImageDrawString(const char* string) {
	while (*string) {
		glutBitmapCharacter(FONT, *string++);
	}
}

int ImageTextWidth(const char* string) {
	int h = 0;
	while (*string) {
		h += glutBitmapWidth(FONT, *string++);
	}
	return h;
}

void ImageSetColor(Color c) {
	glColor3f(1.0 * (c.red % 256 / 256.0), 1.0 * (c.green % 256 / 256.0), 1.0 * (c.blue % 256 / 256.0));
}

void ImageLine(int x1, int y1, int x2, int y2, Color c) {
	ImageSetColor(c);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void ImageFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color c) {
	ImageSetColor(c);
	glBegin(GL_TRIANGLES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
	glEnd();
}

void ImageFilledCircle(int x, int y, int rad, Color c) {
	ImageSetColor(c);
	glBegin(GL_POLYGON);
		for (double a = 0; a <= 2 * pi; a += pi / 12) {
			glVertex2i(x + rad * cos(a), y + rad * sin(a));
		}
	glEnd();
}

void ImageDashedLine(int x1, int y1, int x2, int y2, Color c) {
	ImageSetColor(c);
	glPushAttrib(GL_ENABLE_BIT); 
		glLineStipple(1, 0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		glBegin(GL_LINES);
			glVertex2i(x1, y1);
			glVertex2i(x2, y2);
		glEnd();
	glPopAttrib();
}

void ImageLineWidth(int w) {
	glLineWidth(w);
}

void ImageText(int x1, int y1, char* text, Color c) {
	ImageSetColor(c);
	glPushMatrix();
		glTranslatef(x1, y1, 0);
		glRasterPos2f(0, 0);
		ImageDrawString(text);
	glPopMatrix();
}

void ImageFillScreen(Color c) {
	glClearColor(1.0 * (c.red % 256 / 256.0), 1.0 * (c.green % 256 / 256.0), 1.0 * (c.blue % 256 / 256.0), 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PrintColor(Color c) {
	printf("Color: (%d, %d, %d);\n", c.red, c.green, c.blue);
}

void ImagePixel(int x, int y, Color color) {
	ImageSetColor(color);
	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();
}

void ImageRectangle(int x1, int y1, int x2, int y2, Color color) {
	ImageSetColor(color);
	glBegin(GL_LINE_LOOP);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
	glEnd();
}

void ImageFilledRectangle(int x1, int y1, int x2, int y2, Color color) {
	ImageSetColor(color);
	if (x1 == x2 && y1 == y2) {
		glBegin(GL_POINTS);
			glVertex2i(x1, y1);	
		glEnd();
		return;
	}
	glBegin(GL_QUADS);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
	glEnd();
}

//функция копирования части экрана в другую часть
void ImageCopy(int x, int y, int w, int h, int nx, int ny, int mult) {
	if (ImageCopyBuffer == NULL) {
		//выделяем буффер для копирования части экрана туда
		printf("Allocating imagecopy buffer...\n");
		ImageCopyBuffer = (GLubyte*)malloc(sizeof(GLubyte) * (WIN_WIDTH + 10) * (WIN_HEIGHT + 10) * 3);
		printf("Buffer addr = %d\n", ImageCopyBuffer);
	}

	GLubyte* b = (GLubyte*)ImageCopyBuffer;

	glReadBuffer(GL_BACK);
	glReadPixels(x, WIN_HEIGHT - y - h, w, h, GL_RGB, GL_UNSIGNED_BYTE, b);

	glDrawBuffer(GL_BACK);
	glRasterPos2i(nx, ny + h * mult);
	glPixelZoom(mult, mult);
	glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, b);	
}