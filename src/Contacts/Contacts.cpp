#include <Contacts.h>

/* CONTACT */
Contacts::Contact::Contact(std::string name, std::string number) {
    set_name(name);
    set_number(number);
}
Contacts::Contact::~Contact() { }

std::string Contacts::Contact::get_name() {
    return this->name;
}
std::string Contacts::Contact::get_number() {
    return this->number;
}

void Contacts::Contact::set_name(std::string name) {
    this->name = name;
}
void Contacts::Contact::set_number(std::string number) {
    this->number = number;
}

/* CONTACTS */
Contacts::Contacts() {
    contacts = {};
}
Contacts::~Contacts() { }

std::vector<Contacts::Contact>& Contacts::get() {
    return this->contacts;
}

int Contacts::length() {
    return this->contacts.capacity();
}

Contacts::Contact* Contacts::get_contact(int i) {
    if(0 <= i && i < this->length()) {
        return &(this->contacts[i]);
    }

    return nullptr;
}

Contacts::Contact* Contacts::get_contact(std::string name) {
    for (Contact& c : this->contacts) {
        if(c.get_name() == name)
            return &c;
    }

    return nullptr;
}

void Contacts::add_contact(Contacts::Contact new_contact) {
    contacts.push_back(new_contact);
}

void Contacts::remove_contact(std::string name) {
    for (int i = 0; i < contacts.size(); ++i) {
        if(contacts[i].get_name() == name)
            contacts.erase(contacts.begin() + i);
    }
}

void Contacts::clear() {
    this->contacts.clear();
}