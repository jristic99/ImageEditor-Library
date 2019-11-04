#include "ImageEditor.h"
using namespace std;

// Broj testova je 12. Testovi su indeksirani brojevima 1 - 12. Test indeksiran brojem 0 je rezervisan za studentovo testiranje koda.
const int numberOfTests = 12;

// Globalna promenljiva koja predstavlja indeks testa koji je pokrenut. Koristi se i u prevedenom fajlu 'test.obj'.
int testNumber;

// Deklaracija metode 'test()'. Implementacija se nalazi u prevedenom fajlu 'test.obj'
void test();

// Metoda 'test()' poziva metodu editImage koja vrsi manipulaciju nad slikom u zavisnosti od indeksa testa koji je pokrenut.
// Testovi 1, 9, 10, 11 i 12 vrse manipulaciju nad slikom BlueSquare.bmp
// Testovi 2, 3, 4 i 5 vrse manipulaciju nad slikom FourSquares.bmp
// Testovi 6, 7 i 8 vrse manipulaciju nad slikom OrangeFlower.bmp
// Date slike moraju da se nalaze na istom mestu gde i test.obj fajl
unsigned char* editImage(unsigned char* input) {

	ImageEditor* imageEditor = new ImageEditor;
	unsigned char* output = nullptr;
	switch (testNumber) {
	case 0:
		// MyExample
		/*
			Mesto za proizvoljno testiranje koda.
			Vrsi editovanje slike MyExample.bmp i stvara izlazni fajl MyExample_RESULT.bmp
		*/
		imageEditor->loadImage(input);
		imageEditor->addLayer(); //1
		imageEditor->setActiveColor("#000000");
		imageEditor->fillRect(150, 150, 100, 100);
		imageEditor->blur(15);
		imageEditor->flipHorizontal();
		imageEditor->addLayer(); //2
		imageEditor->blur(15);
		imageEditor->setLayerOpacity(50);
		imageEditor->setActiveColor("#ABAB56");
		imageEditor->fillRect(100, 100, 150, 150);
		imageEditor->eraseRect(36, 40, 55, 55);
		imageEditor->crop(10, 10, 1500, 650);
		imageEditor->flipVertical();
		imageEditor->selectLayer(0);
		imageEditor->invertColors();
		imageEditor->blur(5);
		imageEditor->selectLayer(1);
		imageEditor->deleteLayer();
		imageEditor->selectLayer(0);
		imageEditor->addLayer(); //2
		imageEditor->setActiveColor("#000000");
		imageEditor->setLayerOpacity(75);
		imageEditor->fillRect(150, 150, 150, 150);
		imageEditor->selectLayer(0);
		imageEditor->flipVertical();
		imageEditor->selectLayer(1);
		imageEditor->flipHorizontal();
		imageEditor->selectLayer(1);
		imageEditor->flipHorizontal();
		output = imageEditor->saveImage();
		break;
	case 1:																//check
		// LOAD/SAVE												
		imageEditor->loadImage(input);
		output = imageEditor->saveImage();
		break;
	case 2:																//check
		// FLIP HORIZONTAL											
		imageEditor->loadImage(input);
		imageEditor->flipHorizontal();
		output = imageEditor->saveImage();
		break;
	case 3:																//check
		// FLIP VERTICAL
		imageEditor->loadImage(input);
		imageEditor->flipVertical();
		output = imageEditor->saveImage();
		break;
	case 4:																//check
		// GRAYSCALE
		imageEditor->loadImage(input);
		imageEditor->toGrayScale();
		output = imageEditor->saveImage();
		break;
	case 5:
		// INVERT COLORS												//check
		imageEditor->loadImage(input);
		imageEditor->invertColors();
		output = imageEditor->saveImage();
		break;
	case 6:																//check
		// BLUR WITH SIZE 1
		imageEditor->loadImage(input);
		imageEditor->blur(1);
		output = imageEditor->saveImage();
		break;
	case 7:																//check
		// BLUR WITH SIZE 16
		imageEditor->loadImage(input);
		imageEditor->blur(16);
		output = imageEditor->saveImage();
		break;
	case 8:																//check
		// CROP CENTER OF IMAGE
		imageEditor->loadImage(input);
		imageEditor->crop(32, 32, 64, 64);
		output = imageEditor->saveImage();
		break;
	case 9:																//check
		// NEW LAYER: FILL BLACK RECTANGLE IN THE CENTER OF IMAGE
		imageEditor->loadImage(input);
		imageEditor->addLayer();
		imageEditor->fillRect(32, 32, 64, 64);
		output = imageEditor->saveImage();
		break;
	case 10:
		// NEW LAYER: FILL BLACK RECTANGLE IN THE CENTER OF IMAGE AND DELETE MIDDLE OF IT
		imageEditor->loadImage(input);
		imageEditor->addLayer();
		imageEditor->fillRect(32, 32, 64, 64);
		imageEditor->eraseRect(48, 48, 32, 32);
		output = imageEditor->saveImage();
		break;
	case 11:															//check
		// NEW LAYER: FILL BLACK RECTANGLE IN THE CENTER OF IMAGE AND SET OPACITY OF LAYER TO 50%
		imageEditor->loadImage(input);
		imageEditor->addLayer();
		imageEditor->fillRect(32, 32, 64, 64);
		imageEditor->setLayerOpacity(50);
		output = imageEditor->saveImage();
		break;
	case 12:															//check
		// NEW LAYER: FILL YELLOW RECTANGLE IN THE CENTER OF IMAGE
		imageEditor->loadImage(input);
		imageEditor->addLayer();
		imageEditor->setActiveColor("#FFFF00");
		imageEditor->fillRect(32, 32, 64, 64);
		output = imageEditor->saveImage();
		break;
	}
	delete imageEditor;
	return output;
}

int main() {

	// Ova for petlja ce da pokrene sve testove [0 - 12]. Modifikujte uslov za 'if' da biste pokretali samo odredjene testove (npr: if(testNumber == 1 || testNumber == 5)).
	for(testNumber = 0; testNumber <= numberOfTests; testNumber++)
		if(testNumber == 0) test();

	return 0;
}