#include "json.hpp"

using namespace std;

struct dictionary{
	pair<string,json> info;
    dictionary *next;
};
struct list{
	json info;
	list* next;
};

struct json::list_iterator{
        using iterator_category = forward_iterator_tag;
        using value_type = json;
        using pointer = json*;
        using reference = json&;
        list_iterator(list* ptr):m_ptr(ptr){}
        reference operator*() const{
			return m_ptr->info;
		}
        pointer operator->() const{
			return &(m_ptr->info);
		}
        list_iterator& operator++(){
			m_ptr = m_ptr->next;
			return *this;
		}
        list_iterator operator++(int ){
			list_iterator copy = *this;
			m_ptr = m_ptr->next;
			return copy;
		}
        bool operator==(list_iterator const& rhs) const{
			return m_ptr == rhs.m_ptr;
		}
        bool operator!=(list_iterator const& rhs) const{
			return !(*this == rhs);
		}
        operator bool() const{
			return m_ptr != nullptr;
		}
    private:
        list* m_ptr;
};
struct json::const_list_iterator {
	using iterator_category = forward_iterator_tag;
        using value_type = const json;
        using pointer = json const*;
        using reference = json const&;
        const_list_iterator(list* ptr):m_ptr(ptr){}
        reference operator*() const{
			return m_ptr->info;
		}
        pointer operator->() const{
			return &(m_ptr->info);
		}
        const_list_iterator& operator++(){
			m_ptr = m_ptr->next;
			return *this;
		}
        const_list_iterator operator++(int){
			const_list_iterator copy = *this;
			m_ptr = m_ptr->next;
			return copy;
		}
        bool operator==(const_list_iterator const& rhs) const{
			return m_ptr == rhs.m_ptr;
		}
        bool operator!=(const_list_iterator const& rhs) const{
			return !(*this == rhs);
		}
        operator bool() const{
			return m_ptr != nullptr;
		}
    private:
        list const* m_ptr;
};
struct json::dictionary_iterator{
		using iterator_category = std::forward_iterator_tag;
        using value_type = pair<string,json>;
        using pointer = pair<string,json>*;
        using reference = pair<string,json>&;
        dictionary_iterator(dictionary* ptr):m_ptr(ptr){}
        reference operator*() const{
			return m_ptr->info;
		}
        pointer operator->() const{
			return &(m_ptr->info);
		}
        dictionary_iterator& operator++(){
			m_ptr = m_ptr->next;
			return *this;
		}
        dictionary_iterator operator++(int ){
			dictionary_iterator copy = *this;
			m_ptr = m_ptr->next;
			return copy;
		}
        bool operator==(dictionary_iterator const& rhs) const{
			return m_ptr == rhs.m_ptr;
		}
        bool operator!=(dictionary_iterator const& rhs) const{
			return !(*this == rhs);
		}
        operator bool() const{
			return m_ptr != nullptr;
		}
    private:
        dictionary* m_ptr;
};
struct json::const_dictionary_iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = const pair<string,json>;
        using pointer = pair<string,json> const*;
        using reference = pair<string,json> const&;
        const_dictionary_iterator(dictionary* ptr):m_ptr(ptr){}
        reference operator*() const{
			return m_ptr->info;
		}
        pointer operator->() const{
			return &(m_ptr->info);
		}
        const_dictionary_iterator& operator++(){
			m_ptr = m_ptr->next;
			return *this;
		}
        const_dictionary_iterator operator++(int){
			const_dictionary_iterator copy = *this;
			m_ptr = m_ptr->next;
			return copy;
		}
        bool operator==(const_dictionary_iterator const& rhs) const{
			return m_ptr == rhs.m_ptr;
		}
        bool operator!=(const_dictionary_iterator const& rhs) const{
			return !(*this == rhs);
		}
        operator bool() const{
			return m_ptr != nullptr;
		}
    private:
        dictionary const* m_ptr;
};

enum tipo{Null, Bool, Stringa, Numero, Dizionario, Lista};

struct json::impl{
	list* l_back;
	dictionary* d_back;

    tipo w;
	double n;
    bool b;
    string s;
	dictionary *d;
	list *lista;

    void destroy_list(list* l);
    void destroy_dictionary(dictionary* d);
    void destroy(json& p);
    void azzera(impl* p);
    void copy(json& p, json& s, bool var);
};

void json::impl::azzera(impl* p){
    p->n=0.0;
    p->b=false;
    p->s="";
    p->l_back=nullptr;
    p->d_back=nullptr;

    p->d=nullptr;
    p->lista=nullptr;
    p->w=Null;
}
void json::impl::destroy_list(list* l){
    if(l==nullptr){
        delete l;
        return;
    }
    while(l->next){
        list* mom=l;
        l=l->next;
        delete mom;
    }
    delete l;
}
void json::impl::destroy_dictionary(dictionary* d){
    if(d==nullptr){
        delete d;
        return;
    }
    while(d->next){
        dictionary* mom=d;
        d=d->next;
        delete mom;
    }
    delete d;
}
void json::impl::destroy(json& p){
    if(p.is_list()){
        p.pimpl->destroy_list(p.pimpl->lista);
    }
    if(p.is_dictionary()){
        p.pimpl->destroy_dictionary(p.pimpl->d);
    }
}
void json::impl::copy(json& p, json& s, bool var){
    if(&p==&s){
        return;
    }
    p.pimpl->destroy(p);
    p.pimpl->azzera(p.pimpl);
    if(s.is_null()){
        p.set_null();
        if(var){
             s.pimpl->destroy(s);
        }
        return;
    }
    if(s.is_bool()){
        p.set_bool(s.pimpl->b);
        if(var){
            s.pimpl->destroy(s);
        }
        return;
    }
    if(s.is_string()){
        p.set_string(s.pimpl->s);
        if(var){
            s.pimpl->destroy(s);
        }
        return;
    }
    if(s.is_number()){
        p.set_number(s.pimpl->n);
        if(var){
            s.pimpl->destroy(s);
        }
        return;
    }
    if(s.is_list()){
        p.set_list();
        list* l=s.pimpl->lista;
        while(l!=nullptr){
            p.push_back(l->info);
            if(var){
                list* mom=l;
                s.pimpl->lista=s.pimpl->lista->next;
                l=s.pimpl->lista;
                delete mom;
            }else{
                l=l->next;
            }
        }
        return;
    }
    if(s.is_dictionary()){
        p.set_dictionary();
        dictionary* d=s.pimpl->d;
        while(d!=nullptr){
            p.insert(d->info);
            if(var){
                dictionary* mom=d;
                s.pimpl->d=s.pimpl->d->next;
                d=s.pimpl->d;
                delete mom;
            }else{
                d=d->next;
            }
        }
        return;
    }
}

json::json(){
	pimpl=new impl;
	pimpl->azzera(pimpl);
}
json::json(json const& s):json(){
	*this=s;
}
json::json(json&& s): json(){
	*this=std::move(s);
}
json::~json(){
    pimpl->destroy(*this);
	delete pimpl;
}
json& json::operator=(json const& s){
	if(this == &s){
		return *this;
	}
    if(is_list()){
        pimpl->destroy_list(pimpl->lista);
    }
    if(is_dictionary()){
        pimpl->destroy_dictionary(pimpl->d);
    }
    pimpl->azzera(pimpl);
	if(s.is_null()){
		set_null();
		return *this;
	}
	if(s.is_bool()){
		set_bool(s.pimpl->b);
		return *this;
	}
	if(s.is_string()){
		set_string(s.pimpl->s);
		return *this;
	}
	if(s.is_number()){
		set_number(s.pimpl->n);
		return *this;
	}
    if(s.is_list()){
        set_list();
        list* l=s.pimpl->lista;
        while(l!=nullptr){
            push_back(l->info);
            l=l->next;
        }
        return *this;
    }
    if(s.is_dictionary()){
        set_dictionary();
        dictionary* d=s.pimpl->d;
        while(d!=nullptr){
            insert(d->info);
            d=d->next;
        }
        return *this;
    }
	return *this;
}
json& json::operator=(json&& s){
    pimpl->copy(*this, s, true);
    return *this;
}

bool json::is_list() const{
	if(pimpl->w==Lista){
		return true;
	}
	return false;
}
bool json::is_dictionary() const{
    if(pimpl->w==Dizionario){
        return true;
    }
    return false;
}
bool json::is_string() const{
    if(pimpl->w==Stringa){
        return true;
    }
    return false;
}
bool json::is_number() const{
    if(pimpl->w==Numero){
        return true;
    }
    return false;
}
bool json::is_bool() const{
    if(pimpl->w==Bool){
        return true;
    }
    return false;
}
bool json::is_null() const{
    if(pimpl->w==Null){
        return true;
    }
    return false;
}

json& json::operator[](string const& a){
	if(!is_dictionary()){
		throw json_exception {"Object is not a dictionary"};
	}
	dictionary* d=pimpl->d;
	while(d!=nullptr){
		if(d->info.first==a){
			return d->info.second;
		}
		d=d->next;
	}
	dictionary* mom=new dictionary;
	mom->info.first=a;
	mom->info.second=json();
    if(pimpl->d==nullptr){
        pimpl->d=mom;
        pimpl->d_back=pimpl->d;
        pimpl->w=Dizionario;
    }else{
        d=mom;
        pimpl->d_back=d;
    }
	return mom->info.second;
}
json const& json::operator[](std::string const& a) const{
	if(!is_dictionary()){
		throw json_exception {"Object is not a dictionary"};
	}
	dictionary* d=pimpl->d;
	while(d!=nullptr){
		if(d->info.first==a){
			return d->info.second;
		}
		d=d->next;
	}
	throw json_exception {"The dictionary is empty"};
}

json::list_iterator json::begin_list(){
	if(!is_list()){
		throw json_exception {"Object is not a list"};
	}
	return {pimpl->lista};
}
json::const_list_iterator json::begin_list() const{
	if(!is_list()){
		throw json_exception {"Object is not a list"};
	}
	return {pimpl->lista};
}
json::list_iterator json::end_list(){
	if(!is_list()){
		throw json_exception {"Object is not a list"};
	}
	return {pimpl->l_back->next};
}
json::const_list_iterator json::end_list() const{
	if(!is_list()){
		throw json_exception {"Object is not a list"};
	}
    return {pimpl->l_back->next};
}

json::dictionary_iterator json::begin_dictionary(){
	if(!is_dictionary()){
		throw json_exception {"Object is not a dictionary"};
	}
	return {pimpl->d};
}
json::const_dictionary_iterator json::begin_dictionary() const{
	if(!is_dictionary()){
		throw json_exception {"Object is not a dictionary"};
	}
	return {pimpl->d};
}
json::dictionary_iterator json::end_dictionary(){
	if(!is_dictionary()){
		throw json_exception {"Object is not a dictionary"};
	}
	return {pimpl->d_back->next};
}
json::const_dictionary_iterator json::end_dictionary() const{
	if(!is_dictionary()){
		throw json_exception {"Object is not a dictionary"};
	}
    return {pimpl->d_back->next};
}

double& json::get_number(){
	if(!is_number()){
		throw json_exception {"Object is not a number"};
	}
	return pimpl->n;
}
double const& json::get_number() const{
	if(!is_number()){
		throw json_exception {"Object is not a number"};
	}
	return pimpl->n;
}

bool& json::get_bool(){
	if(!is_bool()){
		throw json_exception {"Object is not a boolean"};
	}
	return pimpl->b;
}
bool const& json::get_bool() const{
	if(!is_bool()){
		throw json_exception {"Object is not a boolean"};
	}
	return pimpl->b;
}

string& json::get_string(){
	if(!is_string()){
		throw json_exception {"Object is not a string"};
	}
	return pimpl->s;
}
string const& json::get_string() const{
	if(!is_string()){
		throw json_exception {"Object is not a string"};
	}
	return pimpl->s;
}

void json::set_string(string const& str){
    if(is_list()){
        pimpl->destroy_list(pimpl->lista);
    }
    if(is_dictionary()){
        pimpl->destroy_dictionary(pimpl->d);
    }
    pimpl->azzera(pimpl);
	pimpl->s=str;
    pimpl->w=Stringa;
}
void json::set_bool(bool b){
    if(is_list()){
        pimpl->destroy_list(pimpl->lista);
    }
    if(is_dictionary()){
        pimpl->destroy_dictionary(pimpl->d);
    }
    pimpl->azzera(pimpl);
    pimpl->b=b;
    pimpl->w=Bool;
}
void json::set_number(double n){
    if(is_list()){
        pimpl->destroy_list(pimpl->lista);
    }
    if(is_dictionary()){
        pimpl->destroy_dictionary(pimpl->d);
    }
    pimpl->azzera(pimpl);
    pimpl->n=n;
    pimpl->w=Numero;

}
void json::set_null(){
    if(is_list()){
        pimpl->destroy_list(pimpl->lista);
    }
    if(is_dictionary()){
        pimpl->destroy_dictionary(pimpl->d);
    }
    pimpl->azzera(pimpl);
    pimpl->w=Null;
}
void json::set_list(){
    if(is_list()){
        pimpl->destroy_list(pimpl->lista);
    }
    if(is_dictionary()){
        pimpl->destroy_dictionary(pimpl->d);
    }
    pimpl->azzera(pimpl);
    pimpl->w=Lista;
}
void json::set_dictionary(){
    if(is_list()){
        pimpl->destroy_list(pimpl->lista);
    }
    if(is_dictionary()){
        pimpl->destroy_dictionary(pimpl->d);
    }
    pimpl->azzera(pimpl);
    pimpl->w=Dizionario;
}

void json::push_front(json const& p){
	if(!is_list()){
		throw json_exception {"Object is not a list"};
	}
    if(pimpl->lista==nullptr){
        pimpl->lista=new list;
        pimpl->lista->info=p;
        pimpl->lista->next=nullptr;
        pimpl->l_back=pimpl->lista;
        return;
    }
    list* mom=new list;
    mom->info=p;
    mom->next=pimpl->lista;
    pimpl->lista=mom;
}
void json::push_back(json const& p){
	if(!is_list()){
		throw json_exception {"Object is not a list"};
	}
    if(pimpl->lista==nullptr){
        pimpl->lista=new list;
        pimpl->lista->info=p;
        pimpl->lista->next=nullptr;
        pimpl->l_back=pimpl->lista;
        return;
    }
    pimpl->l_back->next=new list;
    pimpl->l_back->next->info=p;
    pimpl->l_back->next->next=nullptr;
    pimpl->l_back=pimpl->l_back->next;
}
void json::insert(pair<string, json> const& p){
	if(!is_dictionary()){
		throw json_exception {"Object is not a dictionary"};
	}

    if(pimpl->d==nullptr){
        pimpl->d=new dictionary;
        pimpl->d->info=p;
        pimpl->d->next=nullptr;
        pimpl->d_back=pimpl->d;
        return;
    }
    pimpl->d_back->next=new dictionary;
    pimpl->d_back->next->info=p;
    pimpl->d_back->next->next=nullptr;
    pimpl->d_back=pimpl->d_back->next;
}

void spazi(istream& l){
	while(l.peek()=='\n'||l.peek()=='\t'||l.peek()==' '){
		l.ignore();
	}
}
json istream_json(istream& l);
void ostream_json(ostream& l, json& p);

json istream_null(istream& l){
    spazi(l);
    json p;
    char mom[4];
    l.get(mom, 5);
    string str=mom;
    if(str=="null"){
        p.set_null();
        return p;
    }else{
        throw json_exception {"Istream Null"};
    }
}
json istream_true(istream& l){
    json p;
    char mom[4];
    l.get(mom, 5);
    string str=mom;
    if(str=="true"){
        p.set_bool(true);
        return p;
    }else{
        throw json_exception {"Istream True"};
    }
}
json istream_false(istream& l){
    json p;
    char mom[5];
    l.get(mom, 6);
    string str=mom;
    if(str=="false"){
        p.set_bool(false);
        return p;
    }else{
        throw json_exception {"Istream False"};
    }
}
void istream_num(istream& l, int& count, double& num){
    char zero='0';
    char nove='9';
    if(l.peek()<=zero || l.peek()>=nove){
        if((l.peek()=='.') && (count==-1)){
            l.ignore(0);
            count++;
            return istream_num(l, count, num);
        }else{
            return;
        }
    }
    if(count!=-1){
        count++;
    }
    num=num*10+l.get();
    return istream_num(l, count, num);
}
json istream_string(istream& l){
    l.ignore(1);
    char mom[256];
    l.getline (mom, 256, '"');
    string str=mom;
    json p;
    p.set_string(str);
    return p;
}
void istream_list(istream& l, json& p){
    spazi(l);
    if(l.peek()==']'){
        l.ignore(1);
        return;
    }
    if(l.peek()==','){
        l.ignore(1);
    }
    spazi(l);
    json k=istream_json(l);
    p.push_back(k);
    spazi(l);
    if(l.peek()!=']'&&l.peek()!=','){
        throw json_exception {"Istream Lista"};
    }
    istream_list(l, p);

    /*p.push_back(stream_json(l));
    spazi(l);
    if(l.peek()!=']'||l.peek()!=','){
        throw json_exception {"Istream Lista"};
    }*/
}
void istream_dictionary(istream& l, json& p){
    spazi(l);
    if(l.peek()=='}'){
        l.ignore(1);
        return;
    }
    if(l.peek()==','){
        l.ignore(1);
    }
    spazi(l);
    pair<string, json> k;
    if(l.peek()!='"'){
        throw json_exception {"Istream DictionaryStart"};
    }
    l.ignore(1);
    char mom[256];
    l.getline (mom, 256, '"');
    string str=mom;
    k.first=str;
    spazi(l);
    if(l.get()!=':'){
        throw json_exception {"Istream DictionaryPunti"};
    }
    spazi(l);
    k.second=istream_json(l);
    p.insert(k);
    spazi(l);
    if(l.peek()!='}'&&l.peek()!=','){
        throw json_exception {"Istream DictionaryEnd"};
    }
    istream_dictionary(l, p);
}

json istream_json(istream& l){
    spazi(l);
    if(l.peek()=='n'){
        return istream_null(l);
    }
    if(l.peek()=='t'){
        return istream_true(l);
    }
    if(l.peek()=='f') {
        return istream_false(l);
    }
    char c=l.peek();
    char zero='0';
    char nove='9';
    if(c>=zero && c<=nove){
        double n;
        l>>n;
        json p;
        p.set_number(n);
        spazi(l);
        return p;
    }
    if(l.peek()=='"'){
        return istream_string(l);
    }
    if(l.peek()=='['){
        l.ignore(1);
        json p;
        p.set_list();
        istream_list(l, p);
        return p;
    }
    if(l.peek()=='{'){
        l.ignore(1);
        json p;
        p.set_dictionary();
        istream_dictionary(l, p);
        return p;
    }
    throw json_exception {"Istream General"};
}

void ostream_null(ostream& l, json const& p){
    char c[5]={'n','u','l','l'};
    l<<c;
}
void ostream_bool(ostream& l, json const& p){
    if(p.get_bool()==true){
        char c[5]={'t','r','u','e'};
        l<<c;
    }else{
        char c[6]={'f','a','l','s','e'};
        l<<c;
    }
}
void ostream_string(ostream& l, json const& p){
    l.put('"');
    string str=p.get_string();
    l<<str;
    l.put('"');
}
void ostream_number(ostream& l, json const& p){
    double n=p.get_number();
    l<<n;
}
void ostream_list(ostream& l, json const& p){
    l.put('[');
    auto r=p.begin_list();
    if((r)==nullptr){
        l.put(']');
        return;
    }
    for(auto it=p.begin_list(); it!=p.end_list();++it){
        l<<(*it);
        auto r=it;
        r++;
        if(r!=p.end_list()){
            l.put(',');
        }
    }
    l.put(']');
}
void ostream_dictionary(ostream& l, json const& p){
    l.put('{');
    auto r=p.begin_dictionary();
    if((r)==nullptr){
        l.put(']');
        return;
    }
    l.put('\n');
    for(auto it=p.begin_dictionary(); it!=p.end_dictionary();++it){
        l.put('"');
        string str=it->first;
        l<<str;
        l.put('"');
        l.put(':');
        l<<(it->second);
        auto r=it;
        r++;
        if(r!=p.end_dictionary()){
            l.put(',');
            l.put('\n');
        }
    }
    l.put('\n');
    l.put('}');
}

void ostream_json(ostream& l, json const& p){
    if(p.is_null()){
        ostream_null(l, p);
        return;
    }
    if(p.is_bool()){
        ostream_bool(l, p);
        return;
    }
    if(p.is_string()){
        ostream_string(l, p);
        return;
    }
    if(p.is_number()){
        ostream_number(l, p);
        return;
    }
    if(p.is_list()){
        ostream_list(l, p);
        return;
    }
    if(p.is_dictionary()){
        ostream_dictionary(l, p);
        return;
    }
    throw json_exception {"Ostream General"};
}

ostream& operator<<(ostream& l, json const& p){
    ostream_json(l, p);
    return l;
}
istream& operator>>(istream& l, json& p){
    p.set_null();
	p=istream_json(l);
    return l;
}