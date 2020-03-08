#include "Shader.h"

Uniform::Uniform ( ) : parent ( NULL ) , location ( 0 ) {
	reset ( ) ;
}
Uniform::Uniform ( Shader *shader , const std::string& name , const std::string &additional ) : parent ( NULL ) , location ( 0 ) , name ( name ) , shader ( shader ) , address ( name + additional ) {
	reset ( ) ;
}
Uniform::~Uniform ( ) {
	auto itr = parent->children.find ( std::hash<std::string>{}( name ) ) ;
	if ( itr != parent->children.end ( ) ) parent->children.erase ( itr ) ;
}

uType Uniform::get_type ( ) const {
	return u_UNDEFINED ;
}
Uniform *Uniform::get_parent ( ) const {
	return parent ;
}

void Uniform::reset ( ) const {
}
void Uniform::bind ( ) const {
}

void Uniform::push_child ( const std::string& name , std::vector<Uniform *> child ) {
	children [ std::hash<std::string>{}( name ) ] = child ;
}
void Uniform::push_child ( const std::string &name , Uniform * child ) {
	std::vector<Uniform *> arr ;
	arr.push_back ( child ) ;
	children [ std::hash<std::string>{}( name ) ] = arr ;
}

Uniform &Uniform::operator = ( const void * ) {
	return *this ;
}

std::vector<Uniform *> &Uniform::find ( const std::string &name ) {
	auto itr = children.find ( std::hash<std::string>{}( name ) ) ;
	return std::ref ( itr->second ) ;
}

const std::vector<Uniform *>& Uniform::find ( const std::string& name ) const {
	auto itr = children.find ( std::hash<std::string>{}( name ) ) ;
	return std::ref ( itr->second ) ;
}

// generic uniform form

template<typename T>
gUniform<T>::gUniform ( ) : Uniform ( ) , value ( T ( ) ) {
}
template<typename T>
gUniform<T>::gUniform ( Shader *shader , const std::string &name , const std::string &additional ) : Uniform ( shader , name , additional ) , value ( T ( ) ) {
}
template<typename T>
gUniform<T>::~gUniform ( ) {
}

template<typename T>
void gUniform<T>::bind ( ) const {
	if ( location ) shader->uniform ( location , value ) ;
}
template<typename T>
void gUniform<T>::reset ( )  {
	value = T ( ) ;
}
template<typename T>
gUniform<T> &gUniform<T>::operator = ( const void *value ) {
	this->value = *( const T * ) value ;
	bind ( ) ;
	return *this ;
}

// iUniform

uType iUniform::get_type ( ) const {
	return uType::u_INT ;
}
void iUniform::reset ( ) {
	value = 0 ;
}

// bUniform

uType bUniform::get_type ( ) const {
	return uType::u_BOOL ;
}
void bUniform::reset ( ) {
	value = false ;
}

// fUniform

uType fUniform::get_type ( ) const {
	return uType::u_FLOAT ;
}
void fUniform::reset ( ) {
	value = 0.0f ;
}

// v2Uniform

uType v2Uniform::get_type ( ) const {
	return uType::u_VEC2 ;
}
void v2Uniform::reset ( ) {
	value = glm::vec2 ( 0.0f ) ;
}

// v3Uniform

uType v3Uniform::get_type ( ) const {
	return uType::u_VEC3 ;
}
void v3Uniform::reset ( ) {
	value = glm::vec3 ( 0.0f ) ;
}

// v4Uniform

uType v4Uniform::get_type ( ) const {
	return uType::u_VEC4 ;
}
void v4Uniform::reset ( ) {
	value = glm::vec4 ( glm::vec3 ( 0.0f ) , 1.0f ) ;
}

// m2Uniform

uType m2Uniform::get_type ( ) const {
	return uType::u_MAT2 ;
}
void m2Uniform::reset ( ) {
	value = glm::mat2 ( 1.0f ) ;
}

// m3Uniform

uType m3Uniform::get_type ( ) const {
	return uType::u_MAT3 ;
}
void m3Uniform::reset ( ) {
	value = glm::mat3 ( 1.0f ) ;
}

// m4Uniform

uType m4Uniform::get_type ( ) const {
	return uType::u_MAT4 ;
}
void m4Uniform::reset ( ) {
	value = glm::mat4 ( 1.0f ) ;
}
