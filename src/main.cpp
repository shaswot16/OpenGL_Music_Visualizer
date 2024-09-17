#include <iostream>
#include <stdlib.h>
#include <cmath>
// #include <vector>
#include <string.h>
#include <fftw3.h>	 // fast fourier transform
#include <GL/glut.h> //
#include </usr/include/SFML/System.hpp>
#include "../include/Audio.hpp"
#include "../include/Bar.hpp"

// clock
sf::Clock clock_;
sf::Time timeElapsed = sf::milliseconds(0);
sf::Time fps_delay = sf::milliseconds(33.33);

sf::Time song_delay = sf::milliseconds(450); // nito

std::string audioFilePath = "Audio/ichika-nito.wav";

Audio audio;

Bar bar;
const int barCount = 30;

float bgColor[3] = {0.0f, 0.0f, 0.2f};
float Color[3] = {0.0f, 0.0f, 0.1f};

int scene = 1;

// initializing some fft variables
int real = 0;
int imag = 1;

const int chunkSize = 3200;
int chunksCovered = 0;

int tempAmpFreq = 0;
int powerSpectrum[chunkSize];
int powerSpectrumSmoothed[barCount];

float maxChunkFreq = 0;
float minChunkFreq = 0;
float maxFreq = 0;
float minFreq = 0;

int avgFreq = 0;

fftw_complex input[chunkSize]; // double x[n][2];
fftw_complex output[chunkSize];

float normalize(float num, float min, float max, float maximus)
{
	// zi = (xi – min(x)) / (max(x) – min(x))
	return ((num - min) / (max - min)) * maximus;
}

void updatePowerSpectrum()
{

	maxChunkFreq = 0;
	minChunkFreq = 10000;

	for (int i = 0; i < chunkSize && i + chunksCovered < audio.sampleCount; i++)
	{
		input[i][real] = audio.samples[chunksCovered + i];
		input[i][imag] = 0;
	}

	fftw_plan plan = fftw_plan_dft_1d(chunkSize, input, output, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(plan);

	fftw_destroy_plan(plan);
	fftw_cleanup();

	for (int i = 0; i < chunkSize && i + chunksCovered < audio.sampleCount; i++)
		powerSpectrum[i] = sqrt(pow(output[i + 1][real], 2.0) + pow(output[i + 1][imag], 2.0));

	for (int i = 0; i < barCount; i++)
	{
		powerSpectrumSmoothed[i] = 0;
		for (int j = 0; j < chunkSize && j + chunksCovered < audio.sampleCount; j++)
		{
			powerSpectrumSmoothed[i] += powerSpectrum[j];
		}
		powerSpectrumSmoothed[i] = powerSpectrum[i] / chunkSize;

		if (powerSpectrumSmoothed[i] > maxChunkFreq)
		{
			maxChunkFreq = powerSpectrumSmoothed[i];
		}
		if (powerSpectrumSmoothed[i] < minChunkFreq)
		{
			minChunkFreq = powerSpectrumSmoothed[i];
		}
	}

	chunksCovered += chunkSize;
}

// prolly not the most cleanest way to find max amp of freq in the audio file, but meh
void findMaxFreq()
{

	for (int c = 0; c < (audio.sampleCount / chunkSize); c++)
	{
		updatePowerSpectrum();

		if (maxFreq < maxChunkFreq)
		{
			maxFreq = maxChunkFreq;
		}
	}
	chunksCovered = 0;
}

void displayBars()
{

	glVertexPointer(2, GL_FLOAT, 0, bar.vertices);
	glColorPointer(3, GL_FLOAT, 0, bar.colorsLow);

	glPushMatrix();
	for (int i = 1; i < barCount; i++)
	{

		glTranslatef(bar.barGap, 0.0, 0.0);

		glPushMatrix();
		bar.scaleBar = normalize(powerSpectrumSmoothed[i], minChunkFreq, maxChunkFreq, bar.maxScale);

		glScalef(1.0, bar.scaleBar, 1.0);
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, bar.indices);
		glPopMatrix();
	}
	glPopMatrix();
}

void displayMenuPage()
{
	glColor3f(1.0, 1.0, 1.0);
	std::string description[7] = {
		"   OpenGL Audio Visualizer   ",
		"            by              ",
		"    Shaswot Paudel (35)   ",
		"Bigyan Byanju Shrestha (49) ",
		"",
		"",
	};

	glColor3f(1.0, 1.0, 1.0);
	std::string menuPage[6] = {
		"Choose the audio file:",
		"1. rato_chandra_surya.wav",
		"2. pahilo_prem.wav",
		"3. rumal.wav",
		
	};

	float rasterPosX = -0.19;
	float rasterPosY = 0.3;

	for (int y = 0; y < 7; y++, rasterPosY -= 0.075)
	{

		glRasterPos2f(rasterPosX, rasterPosY);
		for (int x = 0; x < description[y].length(); x++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, description[y][x]);
		}
	}

	for (int y = 0; y < 5; y++, rasterPosY -= 0.075)
	{

		glRasterPos2f(rasterPosX, rasterPosY);
		for (int x = 0; x < menuPage[y].length(); x++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, menuPage[y][x]);
		}
	}
}

void initMusic(std::string audioFile)
{

	audio.loadBuffer(audioFile);

	std::cout << "Sample Rate: " << audio.sampleRate << std::endl;
	std::cout << "Sample Count: " << audio.sampleCount << std::endl;
	std::cout << "Audio Duration: " << audio.duration.asSeconds() << std::endl;

	findMaxFreq();
	std::cout << "Max Freq: " << maxFreq << std::endl;

	audio.song.setVolume(30);
	audio.song.play();
	audio.played = 1;

	sf::sleep(song_delay);
}

void display()
{

	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	if (scene == 1)
	{

		// displayStars();
		displayMenuPage();
	}
	else if (scene == 2)
	{
		// audio.loadBuffer(audioFilePath);

		// displayStars();
		displayBars();

		if (!audio.played)
		{

			initMusic("../Audio/rato_chandra_surya.wav");
		}
	}
	else if (scene == 3)
	{
		// audio.loadBuffer(audioFilePath);

		// displayStars();
		displayBars();

		if (!audio.played)
		{

			initMusic("../Audio/pahilo_prem.wav");
		}
	}
	else if (scene == 4)
	{
		// audio.loadBuffer(audioFilePath);
		sf::Time song_delay = sf::milliseconds(800);

		// displayStars();
		displayBars();

		if (!audio.played)
		{

			initMusic("Audio/rumal.wav");
		}
	}
	else

		// glLoadIdentity();

		glFlush();
	glutSwapBuffers();
}

void keys(unsigned char k, int x, int y)
{

	if (k == ' ')
		scene = 1;
	if (k == '1')
		scene = 2;
	if (k == '2')
		scene = 3;
	if (k == '3')
		scene = 4;
	glutPostRedisplay();
}

void idlefunc()
{

	if (scene > 1)
	{

		updatePowerSpectrum(); // gets new fft values
		// rotateStars();
		// fps_delay.asMilliseconds() - clock_.getElapsedTime().asMilliseconds();
		sf::sleep(fps_delay - clock_.getElapsedTime());
		clock_.restart();
		// scaleBars();
	}

	glutPostRedisplay();
	// std::cout << freqBandArray[0] << std::endl;
}

int main(int argc, char **argv)
{

	// initial filling of fft input array
	updatePowerSpectrum();

	// initializing OpenGL
	glutInit(&argc, argv);
	// glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(900, 900);
	glutCreateWindow("MUSIC VISUALIZER");
	// glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(idlefunc);
	// glutMouseFunc(mouse);
	// glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glutKeyboardFunc(keys);

	glutMainLoop();

	return 0;
}