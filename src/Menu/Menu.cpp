#include <Menu.h>
#include <Contacts.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <conio.h>
#include <vector>

using namespace std;


/* ----- MENU ----- */
Menu::Menu(Contacts& contacts, int width, int height) : contacts(contacts) {
    this->run = true;
    this->width = max(width, MINIMUM_WIDTH);
    this->height = max(height, MINIMUM_HEIGHT);

    this->current_state = new MainMenuState(this);
    this->current_state->begin();
}
Menu::~Menu() { }

int Menu::get_width()   { return this->width;   }
int Menu::get_height()  { return this->height;  }

bool Menu::is_running() { return this->run;             }
void Menu::update()     { this->get_state()->update();  }
void Menu::quit()       { this->run = false;            }


Contacts& Menu::get_contacts()  { return this->contacts;        }
State* Menu::get_state()        { return this->current_state;   }

void Menu::set_state(State* new_state) {
    // End the last state
    this->current_state->end();

    // Begin the new state
    this->current_state = new_state;
    this->current_state->begin();
}


/* ----- STATE ----- */
State::State(Menu* menu): menu(menu) {
    // Allocate the window buffer    
    window = (char *)malloc(sizeof(char) * menu->get_width() * menu->get_height());

    // Reset the memory buffer
    reset_window_buffer();
}

State::~State() {
    // Free the allocated window buffer
    free(window);
}

void State::display_window() {
    clear_display();

    // Some const variables
    const int width = menu->get_width();
    const int height = menu->get_height();
    const string title_name = name;

    // Get the title and the correct sizes
    string title = "|";
    const int name_width = title_name.length() + 2;
    const int left_size = floor((width - name_width) / 2.0);
    const int right_size = ceil((width - name_width) / 2.0);

    // Make the title row
    title.append(left_size, '-');
    title += ' ' + title_name + ' ';
    title.append(right_size, '-');
    title += '|';

    // Print the title row
    cout << title << endl;

    for (size_t row = 0; row < height; ++row)
    {
        string s = "|";
        for (size_t column = 0; column < width; ++column)
        {
            s += window[row*width + column];
        }

        s += "|";
        cout << s << endl;
    }

    string end = "|";
    end.append(width, '-');
    end += "|";
    cout << end << endl;
}

void State::reset_window_buffer() {
    for (size_t i = 0; i < sizeof(char) * menu->get_width() * menu->get_height(); i++)
        window[i] = ' ';
}

void State::set_pixel(int x, int y, char c) {
    const int width = this->menu->get_width();
    const int height = this->menu->get_height();

    if((0 <= x && x < width) && (0 <= y && y < height))
        window[y * width + x] = c;
}

Menu* State::get_current_menu() { return this->menu; }


/* MAIN MENU */
void MainMenuState::begin() { }
void MainMenuState::update() {
    reset_window_buffer();

    // 'Add Contact' Button
    draw_checkbox(this, 3, 1, selected == Selected::add_contact);
    draw_text(this, 6, 1, "Add Contact");

    // 'Show Contacts' Button
    draw_checkbox(this, 3, 3, selected == Selected::show_contacts);
    draw_text(this, 6, 3, "Show Contacts");

    // 'Settings' Button
    draw_checkbox(this, 3, 5, selected == Selected::settings);
    draw_text(this, 6, 5, "Settings");

    // 'Quit' Button
    draw_checkbox(this, 3, 7, selected == Selected::quit);
    draw_text(this, 6, 7, "Quit");

    display_window();

    // Get input
    switch (get_input({
        'q', 
        'w',
        's',
        'e'
    }))
    {
        case 'q':
            this->menu->set_state(new QuitState(this->menu));
            break;
        case 'w':
            selected = (Selected)(max(selected - 1, 0));
            break;
        case 's':
            selected = (Selected)(min(selected + 1, 4));
            break;
        case 'e':
            switch (selected)
            {
                case Selected::add_contact:
                    this->menu->set_state(new AddContactState(this->menu));
                    break;
                case Selected::show_contacts:
                    this->menu->set_state(new ShowContactsState(this->menu));
                    break;
                case Selected::settings:
                    this->menu->set_state(new SettingsState(this->menu));
                    break;
                case Selected::quit:
                    this->menu->set_state(new QuitState(this->menu));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
void MainMenuState::end() { }


/* ADD CONTACT */
void AddContactState::begin() { }
void AddContactState::update() {
    reset_window_buffer();

    // Name input field
    draw_checkbox(this, 2, 1, current_input == &input_name);
    draw_text(this, 5, 1, "Name: " + input_name);

    // Number input field
    draw_checkbox(this, 2, 3, current_input == &input_number);
    draw_text(this, 5, 3, "Number: " + input_number);

    display_window();

    // Get next input
    int c = get_input();

    switch(c) {
        case '\e':
            // If we press the escape key, depending on which input field we are in
            if(current_input == &input_name) {
                this->menu->set_state(new MainMenuState(this->menu));
            }
            else if(current_input == &input_number) {
                current_input = &input_name;
            }
            break;
        case 0x0D:
            // If we press the enter key, depending on which input field we are in
            if(current_input == &input_name) {
                current_input = &input_number;
            }
            else if(current_input == &input_number) {
                // Add the contact itself
                this->menu->get_contacts().add_contact({
                    input_name,
                    input_number
                });

                // Move over to the main menu after adding a contact
                this->menu->set_state(new MainMenuState(this->menu));
            }
            break;
        case 0x8:
            if((*current_input).length() > 0)
                (*current_input).pop_back();
            break;
        default:
            // Check that the character we have gotten is a letter or a number in ASCII
            if( (48 <= c && c <= 57) || 
                (65 <= c && c <= 90) || 
                (97 <= c && c <= 122))
                (*current_input) += c;
            break;
    }
}
void AddContactState::end() { }

/* SHOW CONTACTS */
void ShowContactsState::begin() { }
void ShowContactsState::update() {
    reset_window_buffer();

    std::vector<Contacts::Contact>& contacts = this->menu->get_contacts().get();

    if(contacts.size() == 0) {
        // No contacts exist
        draw_text(this, 2, 1, "No contacts found!");
    }
    else {
        const int height = this->menu->get_height();
        const int width = this->menu->get_width();
        max_contacts = contacts.size();        
        
        // Put contacts in the buffer
        // Clamp the contacts to not exceed the maximum height
        for (int i = 0; i < max_contacts; ++i) {
            // Move 'backwards' 3 spaces when you change the current account selected
            int y_pos = 3*i-(3*current_contact);

            // Checkbox for editing the contact
            draw_checkbox(this, 1, y_pos, i == current_contact);


            // Contact name
            draw_text(this, 3, y_pos, "Name: " + contacts[i].get_name());

            // Contact number
            draw_text(this, 3, y_pos + 1, "Num: " + contacts[i].get_number());
            

            // Border to seperate the contacts
            string border = "";
            border.append(width - 4, '-');

            draw_text(this, 2, y_pos + 2, border);
        }

        // TODO: A kind of progress bar to see how far we've scrolled down
    }

    display_window();

    switch(get_input({
        'q',
        'w',
        's',
        'e',
        'd'
    })) {
        case 'q':
            this->menu->set_state(new MainMenuState(this->menu));
            break;
        case 'w':
            current_contact = max(current_contact - 1, 0);
            break;
        case 's':
            current_contact = min(current_contact + 1, max_contacts - 1);
            break;
        case 'd':
            this->menu->set_state(new DeleteContactState(this->menu, contacts[current_contact]));
            break;
        case 'e':
            this->menu->set_state(new EditContactState(this->menu, contacts[current_contact]));
            break;
        default:
            break;
    }
}
void ShowContactsState::end() { }


/* EDIT CONTACT */
void EditContactState::begin() { }
void EditContactState::update() {
    reset_window_buffer();

    // Name input field
    draw_checkbox(this, 2, 1, current_input == &input_name);
    draw_text(this, 5, 1, "Name: " + input_name);

    // Number input field
    draw_checkbox(this, 2, 3, current_input == &input_number);
    draw_text(this, 5, 3, "Number: " + input_number);

    display_window();

    // Get next input
    int c = get_input();

    switch(c) {
        case '\e':
            // If we press the escape key, depending on which input field we are in
            if(current_input == &input_name) {
                this->menu->set_state(new MainMenuState(this->menu));
            }
            else if(current_input == &input_number) {
                current_input = &input_name;
            }
            break;
        case 0x0D:
            // If we press the enter key, depending on which input field we are in
            if(current_input == &input_name) {
                current_input = &input_number;
            }
            else if(current_input == &input_number) {
                // Edit the contact
                to_edit.set_name(input_name);
                to_edit.set_number(input_number);

                // Move over to the main menu after adding a contact
                this->menu->set_state(new MainMenuState(this->menu));
            }
            break;
        case 0x8:
            if((*current_input).length() > 0)
                (*current_input).pop_back();
            break;
        default:
            // Check that the character we have gotten is a letter or a number in ASCII
            if( (48 <= c && c <= 57) || 
                (65 <= c && c <= 90) || 
                (97 <= c && c <= 122))
                (*current_input) += c;
            break;
    }

}
void EditContactState::end() { }


/* REMOVE CONTACT */
void DeleteContactState::begin() { }
void DeleteContactState::update() {
    reset_window_buffer();

    // Question label for deleting the contact
    draw_text(this, 3, 1, "Delete contact '" + to_delete.get_name() + "'?");

    // 'Cancel' Button
    draw_checkbox(this, 3, 3, !want_to_delete);
    draw_text(this, 6, 3, "Cancel");

    // 'Delete' Button
    draw_checkbox(this, 3, 5, want_to_delete);
    draw_text(this, 6, 5, "Delete");

    display_window();

    switch (get_input({
        'w',
        's',
        'e',
        'q'
    }))
    {
        case 'q':
            this->menu->set_state(new ShowContactsState(this->menu));
            break;
        case 'w':
        case 's':
            want_to_delete = !want_to_delete;
            break;
        case 'e':
            // Delete in case the user want's to delete
            if(want_to_delete)
                this->menu->get_contacts().remove_contact(to_delete.get_name());
            
            // Redirect back to the show contacts state
            this->menu->set_state(new ShowContactsState(this->menu));
            break;
        default:
            break;
    }
}
void DeleteContactState::end() { }


/* SETTINGS */
void SettingsState::begin() { }
void SettingsState::update() {
    reset_window_buffer();

    // 'Save' Button
    draw_checkbox(this, 3, 1, selected == Selected::Save);
    draw_text(this, 6, 1, "Save");

    // 'Load' Button
    draw_checkbox(this, 3, 3, selected == Selected::Load);
    draw_text(this, 6, 3, "Load");

    // 'Back' Button
    draw_checkbox(this, 3, 5, selected == Selected::Back);
    draw_text(this, 6, 5, "Back");


    display_window();

    switch (get_input({
        'q',
        'w',
        's',
        'e'
    }))
    {
        case 'q':
            this->menu->set_state(new MainMenuState(this->menu));
            break;
        case 'w':
            selected = (Selected)(max(selected - 1, 0));
            break;
        case 's':
            selected = (Selected)(min(selected + 1, 3));
            break;
        case 'e':
            // Make sure the file exists
            fstream file;

            file.open(FILE_NAME, ios::out | ios::app);
            file.close();

            string s = "";

            switch (selected)
            {
                case Save:
                    // Reset the file
                    file.open(FILE_NAME, ios::out | ios::trunc);

                    for (Contacts::Contact c : this->menu->get_contacts().get()) {
                        s += c.get_name() + ';' + c.get_number() + "\n";
                    }

                    file << s;

                    file.close();
                    break;
                case Load:
                    file.open(FILE_NAME, ios::in);

                    // Clear the current list in memory
                    this->menu->get_contacts().clear();

                    while(file >> s) {
                        const string name = s.substr(0, s.find(';'));
                        const string number = s.substr(s.find(';'), s.length());
                        
                        this->menu->get_contacts().add_contact({
                            name,
                            number
                        });
                    }

                    file.close();
                    break;
                case Back:
                    break;
            }
            this->menu->set_state(new MainMenuState(this->menu));
            break;
    }
}
void SettingsState::end() { }


/* QUIT */
void QuitState::begin() {
    this->menu->quit();
}
void QuitState::update() { }
void QuitState::end() { }


/* ----- HELPER ----- */
int get_input(std::vector<int> acceptable_keys) {
    int input{};

    while(true){
        // Windows Specific, get next character pressed
        input = _getch();

        // If acceptable_keys is empty, accept every key press
        if(acceptable_keys.size() == 0)
            break;
        
        // Check if the key pressed is inside the acceptable_keys
        bool found = false;
        for (int key : acceptable_keys) {
            if(key == input)
                found = true;
        }
        if(found)
            break;
    }

    return input;
}

void clear_display() {
    for (size_t i = 0; i < MINIMUM_HEIGHT; i++)
        cout << endl;
}

/* HELPER DRAW FUNCTIONS */
void draw_empty_checkbox(State *current_state, int x, int y) {
    current_state->set_pixel(x + 1, y, ']'); // Right border
    current_state->set_pixel(x - 1, y, '['); // Left border
}

void draw_filled_checkbox(State *current_state, int x, int y) {
    draw_empty_checkbox(current_state, x, y);

    current_state->set_pixel(x, y, 'x');
}

void draw_checkbox(State *current_state, int x, int y, bool filled) {
    if(filled)
        draw_filled_checkbox(current_state, x, y);
    else
        draw_empty_checkbox(current_state, x, y);
}

void draw_text(State *current_state, int x, int y, std::string text) {
    int text_length = text.length();
    int width = current_state->get_current_menu()->get_width();

    if (x + text_length >= width) {
        // Text is too long, shorten it
        text = text.substr(0, width - x - 2);
        text += "..";
    }

    for (int i = 0; i < text.length(); ++i)
        current_state->set_pixel(x + i, y, text[i]);
}