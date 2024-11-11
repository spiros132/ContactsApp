#pragma once
#include <Contacts.h>

#include <string>
#include <vector>

#define STATE(NAME, DISPLAYNAME) class NAME: public State { \
    public: \
    NAME(Menu* menu) : State(menu) { name = #DISPLAYNAME; } \
    void begin() override; \
    void update() override; \
    void end() override; \
};

#define MINIMUM_WIDTH 15
#define MINIMUM_HEIGHT 10

#define FILE_NAME "contacts.txt"

/* HELPER INPUT FUNCTIONS */
/**
 * @brief Get the next input of the keyboard, and only return it if it exists inside the acceptable_keys array
 * 
 * @param acceptable_keys the acceptable_keys for this function to return anything
 * @return the current key pressed
 */
int get_input(std::vector<int> acceptable_keys = {});

/**
 * @brief Clear the display
 * 
 */
void clear_display();

class Menu;

/* State classes */
class State {
    protected:
    Menu* menu;
    char* window;
    std::string name;

    public:
    State(Menu* menu);
    ~State();


    /**
     * @brief What happens at the beginning when you choose this state
     * 
     */
    virtual void begin() = 0;

    /**
     * @brief What happens when the state updates
     * 
     */
    virtual void update() = 0;

    /**
     * @brief What happens at the end of this state when you choose another state
     * 
     */
    virtual void end() = 0;


    /**
     * @brief Display the current buffer to the window
     * 
     */
    void display_window();

    /**
     * @brief Clear the buffer to it's 'normal' setting
     * 
     */
    void reset_window_buffer();

    /**
     * @brief Set the pixel in the buffer to this type of character
     * 
     * @param x the x coordinate of the pixel
     * @param y the y coordinate of the pixel
     * @param c the character to display
     */
    void set_pixel(int x, int y, char c);


    /**
     * @brief Get the current menu that this state exists on
     * 
     * @return the current menu
     */
    Menu* get_current_menu();
};

class MainMenuState: public State {
    private:
    enum Selected {
        add_contact,
        show_contacts,
        settings,
        quit
    } selected = Selected::add_contact;

    public:
    MainMenuState(Menu* menu) : State(menu) { name = "Main Menu"; }
    void begin() override;
    void update() override;
    void end() override;
};

class AddContactState: public State {
    private:
    std::string input_name = "";
    std::string input_number = "";
    std::string* current_input = &input_name;

    public:
    AddContactState(Menu* menu) : State(menu) { name = "Add Contact"; }
    void begin() override;
    void update() override;
    void end() override;
};

class EditContactState: public State {
    private:
    Contacts::Contact& to_edit;
    std::string input_name = "";
    std::string input_number = "";
    std::string* current_input = &input_name;
    public:
    EditContactState(Menu* menu, Contacts::Contact& to_edit) : 
        State(menu), to_edit(to_edit) {
        name = "Edit Contact"; 
        input_name = to_edit.get_name(); 
        input_number = to_edit.get_number();
    }
    void begin() override;
    void update() override;
    void end() override;
};

class DeleteContactState: public State {
    private:
    Contacts::Contact& to_delete;
    bool want_to_delete = false;
    public:
    DeleteContactState(Menu* menu, Contacts::Contact& to_delete) : State(menu), to_delete(to_delete) { name = "Remove Contact"; }
    void begin() override;
    void update() override;
    void end() override;
};

class ShowContactsState: public State {
    private:
    int current_contact = 0;
    int max_contacts = 0;

    public:
    ShowContactsState(Menu* menu) : State(menu) { name = "Contacts"; }
    void begin() override;
    void update() override;
    void end() override;
};

class SettingsState: public State {
    private:
    enum Selected {
        Save,
        Load,
        Back
    } selected = Save;

    public:
    SettingsState(Menu* menu) : State(menu) { name = "Settings"; }
    void begin() override;
    void update() override;
    void end() override;
};

STATE(QuitState, Quit)

/* MENU Class */
class Menu {
    private:
    Contacts& contacts;
    State* current_state;
    bool run;
    int width;
    int height;

    public:
    /**
     * @brief Construct a new Menu object
     * 
     * @param contacts The contacts that are going to be displayed
     * @param width The width of the menu
     * @param height The height of the menu
     */
    Menu(Contacts& contacts, int width, int height);
    ~Menu();

    /**
     * @brief Get the width of the Menu
     * 
     * @return the width
     */
    int get_width();

    /**
     * @brief Get the height of the Menu
     * 
     * @return the height
     */
    int get_height();

    /**
     * @brief Update the current state
     * 
     */
    void update();

    /**
     * @brief Check if the main menu is still running
     * 
     * @return if it's still running 
     */
    bool is_running();

    /**
     * @brief Function to run in case we want to quit the menu
     * 
     */
    void quit();

    /**
     * @brief Get the current state of the menu
     * 
     * @return The state of the menu
     */
    State* get_state();

    /**
     * @brief Get the current contacts object
     * 
     * @return The contacts object reference
     */
    Contacts& get_contacts();

    /**
     * @brief Set the current state of the menu
     * 
     * @param new_state The new state
     */
    void set_state(State* new_state);
};

/* HELPER DRAW FUNCTIONS */
/**
 * @brief Draw an empty checkbox
 * 
 * @param current_state the state where to draw the checkbox
 * @param x the x position
 * @param y the y position
 */
void draw_empty_checkbox(State* current_state, int x, int y);

/**
 * @brief Draw a filled checkbox
 * 
 * @param current_state the state where to draw the checkbox
 * @param x the x position
 * @param y the y position
 */
void draw_filled_checkbox(State* current_state, int x, int y);

/**
 * @brief Draw a checkbox that's filled if the filled boolean is true otherwise it's empty
 * 
 * @param current_state the state where to draw the checkbox
 * @param x the x position
 * @param y the y position
 * @param filled if the checkbox is filled or not
 */
void draw_checkbox(State* current_state, int x, int y, bool filled);

/**
 * @brief Draw text 
 * 
 * @param current_state the state where to draw the text
 * @param x the x position
 * @param y the y position
 * @param text the text to display
 */
void draw_text(State* current_state, int x, int y, std::string text);