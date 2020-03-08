#pragma once
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <map>

#include <glm/glm.hpp>

class Shader ;
class Uniform ;

enum uType {
	u_BOOL ,
	u_INT ,
	u_FLOAT ,
	u_VEC2 ,
	u_VEC3 ,
	u_VEC4 ,
	u_MAT2 ,
	u_MAT3 ,
	u_MAT4 ,
	u_UNDEFINED
};

// Generic Uniform

class Uniform {
protected:
	int location ;

	std::string name ;
	std::string address ;

	Uniform *parent ;
	Shader *shader ;

	std::unordered_map<size_t , std::vector<Uniform *>> children ;
public:
	Uniform ( ) ;
	Uniform ( Shader *shader , const std::string& name , const std::string& additional = "" ) ;
	~Uniform ( ) ;

	void push_child ( const std::string &name , std::vector<Uniform *> child ) ;
	void push_child ( const std::string &name , Uniform * child ) ;

	const std::vector<Uniform *>& find ( const std::string &name ) const ;
	std::vector<Uniform *>& find ( const std::string &name ) ;

	inline void set_location ( int location ) { this->location = location ; }

	inline const std::string &get_name ( ) const { return name ; }
	inline const std::string &get_address ( ) const { return address ; }

	virtual void bind ( ) const ;

	virtual uType get_type ( ) const ;
	virtual void reset ( ) const ;

	Uniform *get_parent ( ) const ;

	virtual Uniform &operator = ( const void *value ) ;
};

template<typename T>
class gUniform : public Uniform {
protected:
	T value ;
public:
	gUniform ( ) ;
	gUniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) ;
	~gUniform ( ) ;

	virtual uType get_type ( ) const = 0 ;
	virtual void reset ( ) ;
	
	void bind ( ) const ;

	gUniform &operator = ( const void *value ) ;
};

// integer uniform

class iUniform : public gUniform<int> {
public:
	iUniform ( ) : gUniform<int> ( ) { }
	iUniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) : gUniform<int> ( shader , name , additional ) { }
	~iUniform ( ) { }

	uType get_type ( ) const ;
	void reset ( ) ;
};

// boolean uniform

class bUniform : public gUniform<bool> {
public:
	bUniform ( ) : gUniform<bool> ( ) { }
	bUniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) : gUniform<bool> ( shader , name , additional ) { }
	~bUniform ( ) { }

	uType get_type ( ) const ;
	void reset ( ) ;
};

// float uniform

class fUniform : public gUniform<float> {
public:
	fUniform ( ) : gUniform<float> ( ) { }
	fUniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) : gUniform<float> ( shader , name , additional ) { }
	~fUniform ( ) { }

	uType get_type ( ) const ;
	void reset ( ) ;
};

// vec2 uniform

class v2Uniform : public gUniform<glm::vec2> {
public:
	v2Uniform ( ) : gUniform<glm::vec2> ( ) { }
	v2Uniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) : gUniform<glm::vec2> ( shader , name , additional ) { }
	~v2Uniform ( ) { }

	uType get_type ( ) const ;
	void reset ( ) ;
};

// vec3 uniform

class v3Uniform : public gUniform<glm::vec3> {
public:
	v3Uniform ( ) : gUniform<glm::vec3> ( ) { }
	v3Uniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) : gUniform<glm::vec3> ( shader , name , additional ) { }
	~v3Uniform ( ) { }

	uType get_type ( ) const ;
	void reset ( ) ;
};

// vec4 uniform

class v4Uniform : public gUniform<glm::vec4> {
public:
	v4Uniform ( ) : gUniform<glm::vec4> ( ) { }
	v4Uniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) : gUniform<glm::vec4> ( shader , name , additional ) { }
	~v4Uniform ( ) { }

	uType get_type ( ) const ;
	void reset ( ) ;
};

// mat2 uniform

class m2Uniform : public gUniform<glm::mat2> {
public:
	m2Uniform ( ) : gUniform<glm::mat2> ( ) { }
	m2Uniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) : gUniform<glm::mat2> ( shader , name , additional ) { }
	~m2Uniform ( ) { }

	uType get_type ( ) const ;
	void reset ( ) ;
};

// mat3 uniform

class m3Uniform : public gUniform<glm::mat3> {
public:
	m3Uniform ( ) : gUniform ( ) { }
	m3Uniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) : gUniform<glm::mat3> ( shader , name , additional ) { }
	~m3Uniform ( ) { }

	uType get_type ( ) const ;
	void reset ( ) ;
};

// mat4 uniform

class m4Uniform : public gUniform<glm::mat4> {
public:
	m4Uniform ( ) : gUniform<glm::mat4> ( ) { }
	m4Uniform ( Shader *shader , const std::string &name , const std::string &additional = "" ) : gUniform<glm::mat4> ( shader , name , additional ) { }
	~m4Uniform ( ) { }

	uType get_type ( ) const ;
	void reset ( ) ;
};

class Shader {
	unsigned int program ;
	void load ( const std::string &vert , const std::string &frag ) ;
	void load_program ( unsigned int targetShader , const std::string &file ) ;
public:
	Shader ( ) ;
	Shader ( const std::string& vert , const std::string& frag ) ;
	~Shader ( ) ;

	std::map< std::string , Uniform * > uniforms ;

	int get_location ( const std::string &name ) const ;

	void config ( const std::string &conf ) ;

	// virtual void transform ( const Transform &model ) ;
	// virtual void camera ( const Transform &camera ) ;
	// virtual void material ( const Material &material ) ;

	void bind ( ) ;
	void unbind ( ) ;

	void uniform ( const char *name , bool data ) const ;
	void uniform ( const char *name , int data ) const ;
	void uniform ( const char *name , float data ) const ;
	void uniform ( const char *name , glm::vec2 data ) const ;
	void uniform ( const char *name , glm::vec3 data ) const ;
	void uniform ( const char *name , glm::vec4 data ) const ;
	void uniform ( const char *name , glm::mat2 data ) const ;
	void uniform ( const char *name , glm::mat3 data ) const ;
	void uniform ( const char *name , glm::mat4 data ) const ;

	void uniform ( int upos , bool data ) const ;
	void uniform ( int upos , int data ) const ;
	void uniform ( int upos , float data ) const ;
	void uniform ( int upos , glm::vec2 data ) const ;
	void uniform ( int upos , glm::vec3 data ) const ;
	void uniform ( int upos , glm::vec4 data ) const ;
	void uniform ( int upos , glm::mat2 data ) const ;
	void uniform ( int upos , glm::mat3 data ) const ;
	void uniform ( int upos , glm::mat4 data ) const ;
};

