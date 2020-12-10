#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "olcNoiseMaker.h"

using namespace std;

atomic<double> freq = 0.0;
bool shiftPressed = false;
const int NUM_NOTES = 13;
sf::RectangleShape *notes = new sf::RectangleShape[NUM_NOTES];
sf::Text* noteNames = new sf::Text[NUM_NOTES];
string noteLetters[13] = { "A3", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A4"}; //Array of the note letters for drawing text
char keyboardLetters[13] = { 'A', 'W', 'S', 'D', 'R', 'F', 'T', 'G', 'H', 'U', 'J', 'I', 'K' }; //Array of corresponding keyboard keys

// Method to model and return a sin wave at specific time
double soundWave(double time) {
    double sinWave = 0.3 * (sin(freq * 2 * PI * time) + sin(freq * 2 * PI * time));

    // If shift is pressed, uses square sin wave rather than regular sin wave
    if (shiftPressed == true) {
        if (sinWave > 0) {
            return 0.03;
        }
        else {
            return -0.03;
        }
    }
    else {
        return sinWave;
    }


}


int main() {
    // SFML window and shape generation
    sf::RenderWindow window(sf::VideoMode(1080, 640), "please work this time is my last hope");

    // load font and check for errors
    sf::Font font;
    if (!font.loadFromFile("font/CaviarDreams.ttf")) {
        cout << "uh oh busted" << endl;
    }
    
    /* For loop creates rectangles for each note I want (in this case 13) and places them 5 pixels apart. Offsets y position of sharp notes
    Adds each one to notes array for later reference
    Also creates a text object to draw the note names/letters on each key*/
    for (int i = 0; i < NUM_NOTES; i++) {
        if (noteLetters[i].find('#') == string::npos) { // If there is a sharp in the name, offset it by 150 pixels up
            sf::RectangleShape note(sf::Vector2f(50.00, 150.0));
            note.setPosition(200.00 + (i * 55.00), 300.00);
            notes[i] = note;

            sf::Text text;
            text.setFont(font);
            text.setFillColor(sf::Color::Black);
            stringstream keyboardText;
            keyboardText << noteLetters[i] << "\n\n(" << keyboardLetters[i] << ")";
            text.setString(keyboardText.str());
            text.setCharacterSize(25);
            text.setPosition(200.00 + (i * 55.00) + 10, 300.00 + 10);
            noteNames[i] = text;
        }
        else {
            sf::RectangleShape note(sf::Vector2f(50.00, 200.0));
            note.setPosition(200.00 + (i * 55.00), 150.00);
            notes[i] = note;

            sf::Text text;
            text.setFont(font);
            text.setFillColor(sf::Color::Black);
            stringstream keyboardText;
            keyboardText << noteLetters[i] << "\n\n(" << keyboardLetters[i] << ")";
            text.setString(keyboardText.str());
            text.setCharacterSize(25);
            text.setPosition(200.00 + (i * 55.00) + 10, 200.00 + 10);
            noteNames[i] = text;
        }
    }

    // Create vector of computer sound devices using method from olcNoiseMaker
    vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

    // Create link between this program and user's sound device(s)
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    // Send our created sound wave to sound device
    sound.SetUserFunction(soundWave);

    while (true) {
        sf::Event event;

        bool keyPressed = false;

        // Detects if shift is pressed
        if (GetAsyncKeyState(VK_LSHIFT)) {
            shiftPressed = true;
        }
        else {
            shiftPressed = false;
        }

        // Checks which key is pressed and determines the frequency based on key
        
        for (int i = 0; i < NUM_NOTES; i++) {
            if (GetAsyncKeyState(keyboardLetters[i]) && 0x8000) {
                freq = pow(2.0, (i / 12.0)) * 220.0;    //multiply base freq by 2^(n/12) for each half step of the octave
                keyPressed = true;
                notes[i].setFillColor(sf::Color::Green);    // makes the key green when pressed for feedback
            }
        }

        // if no keys pressed, don't make noise
        if (!keyPressed) {
            freq = 0;
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(219, 173, 173, 255));
        // for loop draws every note + text in notes array
        for (int i = 0; i < NUM_NOTES; i++) {
            if (!keyPressed) {
                notes[i].setFillColor(sf::Color::White);
            }
            window.draw(notes[i]);
            window.draw(noteNames[i]);
        }
        window.display();
    }


    return 0;
}
















