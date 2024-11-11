#pragma once

#include <vector>
#include <string>

/**
 * @brief The class that contains all the contacts
 * 
 */
class Contacts {
    public:
    /**
     * @brief A class that contains a singular contact
     * 
     */
    class Contact {
        public:
        /**
         * @brief Construct a new Contact object
         * 
         * @param name The name of the contact
         * @param number The number of the contact
         */
        Contact(std::string name = "", std::string number = "");
        ~Contact();

        /**
         * @brief Get the name of the contact
         * 
         * @return The name of the contact
         */
        std::string get_name();

        /**
         * @brief Get the number of the contact
         * 
         * @return The number of the contact
         */
        std::string get_number();

        /**
         * @brief Set the name of the contact
         * 
         * @param name The new name of the contact
         */
        void set_name(std::string name);

        /**
         * @brief Set the number of the contact
         * 
         * @param number The new number of the contact
         */
        void set_number(std::string number);

        private:
        std::string name;
        std::string number;
    };

    Contacts();
    ~Contacts();

    /**
     * @brief Get all the contacts
     * 
     * @return The contacts
     */
    std::vector<Contact>& get();

    /**
     * @brief Returns the lenght of the contact list
     * 
     * @return The length of the contact list
     */
    int length();

    /**
     * @brief Get a contact from the list
     * 
     * @param i The index of the contact
     * @return The contact
     */
    Contact* get_contact(int i);
    /**
     * @brief Get a contact from the list
     * 
     * @param name The name of the contact
     * @return The contact
     */
    Contact* get_contact(std::string name);

    /**
     * @brief Add a new contact
     * 
     * @param new_contact The new contact
     */
    void add_contact(Contact new_contact);

    /**
     * @brief Remove an existing contact
     * 
     * @param name The existing contacts name
     */
    void remove_contact(std::string name);

    /**
     * @brief Clear the list of all the contacts, aka remove everything
     * 
     */
    void clear();

    private:
    std::vector<Contact> contacts;
};