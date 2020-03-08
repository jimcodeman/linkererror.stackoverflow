#define _CRT_SECURE_NO_WARNINGS
#include "Shader.h"

#include <gl/glew.h>
#include <gl/GL.h>

#include <stack>
#include <Windows.h>

void texColor ( int color ) {
	HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );
	SetConsoleTextAttribute ( hConsole , color );
}

Shader::Shader ( ) : program ( 0 ) {
}

Shader::Shader ( const std::string& vert , const std::string& frag ) {
	load ( vert , frag ) ;
}

Shader::~Shader ( ) {
}

void Shader::load ( const std::string& vert , const std::string& frag ) {
	GLuint vertShader , fragShader;

	load_program ( vertShader = glCreateShader ( GL_VERTEX_SHADER ) , vert );
	load_program ( fragShader = glCreateShader ( GL_FRAGMENT_SHADER ) , frag );

	program = glCreateProgram ( );

	glAttachShader ( program , vertShader );
	glAttachShader ( program , fragShader );

	glLinkProgram ( program );

	int result;
	glGetProgramiv ( program , GL_LINK_STATUS , &result );

	if ( result != GL_TRUE ) {

		int logLength;
		glGetProgramiv ( program , GL_INFO_LOG_LENGTH , &logLength );
		char *ProgramError = new char [ ( size_t ) ( logLength + 1u ) ];
		glGetProgramInfoLog ( program , logLength , NULL , ProgramError );

		printf ( "Error Linking Program , Error Details :\n%s\n" , ProgramError );
	}

	glDeleteShader ( vertShader );
	glDeleteShader ( fragShader );
}
void Shader::load_program ( unsigned int targetShader , const std::string &file ) {
	FILE *in ;

	char c ;
	std::string input ;

	fopen_s ( &in , file.c_str ( ) , "r" ) ;

	if ( in ) {
		while ( fscanf_s ( in , "%c" , &c ) > EOF ) input += c ;
		fclose ( in ) ;
	}
	else {
		printf ( "Shader>Error opening file : %s\n" , file.c_str ( ) ) ;
	}

	GLchar *text = new GLchar [ input.size ( ) + 1 ];

	for ( int i = 0; i < input.size ( ); i++ ) text [ i ] = input [ i ];
	text [ input.size ( ) ] = 0;

	glShaderSource ( targetShader , 1 , &text , NULL );
	glCompileShader ( targetShader );

	int result;
	glGetShaderiv ( targetShader , GL_COMPILE_STATUS , &result );

	if ( result != GL_TRUE ) {
		int logLength;
		glGetShaderiv ( targetShader , GL_INFO_LOG_LENGTH , &logLength );
		char *ShaderError = new char [ ( size_t ) ( logLength + 1u ) ];
		glGetShaderInfoLog ( targetShader , logLength , NULL , ShaderError );

		printf ( "Error Compiling [%s] , Error Details :\n%s\n" , file.c_str ( ) , ShaderError );
	}
}

void Shader::config ( const std::string &fconfig ) {
	std::stack<std::vector<Uniform *>> ustack ;

	FILE *conf = fopen ( fconfig.c_str ( ) , "r" ) ;
	char *word_char = new char [ 1024 ] ;

	while ( fscanf_s ( conf , "%s" , &word_char , 1024 ) != EOF ) {
		
		std::string word = word_char ;

		if ( word == "}" ) {
			ustack.pop ( ) ;
			continue ;
		}

		size_t length = 1 ;

		std::string attrib ;

		uType uniform_type = u_UNDEFINED ;

		std::vector<Uniform *> arr ;

		while ( fscanf_s ( conf , "%s" , &word_char , 1024 ) != EOF ) {

			attrib = word_char ;

			if ( attrib == "{" ) break ;

			else if ( attrib == "array" ) {
				assert ( fscanf_s ( conf , "%d" , &length ) != EOF ) ;
			}

			else if ( attrib == "int" ) uniform_type = u_INT ;
			else if ( attrib == "bool" ) uniform_type = u_BOOL ;
			else if ( attrib == "float" ) uniform_type = u_FLOAT ;
			else if ( attrib == "vec2" ) uniform_type = u_VEC2 ;
			else if ( attrib == "vec3" ) uniform_type = u_VEC3 ;
			else if ( attrib == "vec4" ) uniform_type = u_VEC4 ;
			else if ( attrib == "mat2" ) uniform_type = u_MAT2 ;
			else if ( attrib == "mat3" ) uniform_type = u_MAT3 ;
			else if ( attrib == "mat4" ) uniform_type = u_MAT4 ;

			else {
				texColor ( 4 ) ;
				printf ( "COMPILATION ERROR NEAR \"" ) ;
				texColor ( 2 ) ;
				printf ( "%s" , word.c_str ( ) ) ;
				texColor ( 4 ) ;
				printf ( "\"" ) ;
				texColor ( 3 ) ;
				printf ( " on attrib %s\n" , attrib.c_str ( ) ) ;
				texColor ( 7 ) ;
			}

		}

		for ( int i = 0 ; i < length ; i++ ) {
			for ( auto p : ustack.top ( ) ) {
				std::string additional ;

				if ( ustack.size ( ) > 1 ) additional = "[" + std::to_string ( i ) + "]" ;

				Uniform *uniform = NULL ;

				texColor ( 11 ) ;
				printf ( "NEW UNIFORM>" ) ;
				texColor ( 9 ) ;
				printf ( " %s " , word.c_str ( ) ) ;
				texColor ( 7 ) ;

				switch ( uniform_type ) {
					case u_UNDEFINED: {
						uniform = new Uniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "struct / undefined type" ) ;
						texColor ( 7 ) ;
					}break;
					case u_INT: {
						uniform = new iUniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "int type" ) ;
						texColor ( 7 ) ;
					}break;
					case u_BOOL: {
						uniform = new bUniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "bool type" ) ;
						texColor ( 7 ) ;
					}break;
					case u_FLOAT: {
						uniform = new fUniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "float type" ) ;
						texColor ( 7 ) ;
					}break;
					case u_VEC2: {
						uniform = new v2Uniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "vec2 type" ) ;
						texColor ( 7 ) ;
					}break;
					case u_VEC3: {
						uniform = new v3Uniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "vec3 type" ) ;
						texColor ( 7 ) ;
					}break;
					case u_VEC4: {
						uniform = new v4Uniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "vec4 type" ) ;
						texColor ( 7 ) ;
					}break;
					case u_MAT2: {
						uniform = new m2Uniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "mat2 type" ) ;
						texColor ( 7 ) ;
					}break;
					case u_MAT3: {
						uniform = new m3Uniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "mat3 type" ) ;
						texColor ( 7 ) ;
					}break;
					case u_MAT4: {
						uniform = new m4Uniform ( this , word , additional ) ;
						texColor ( 2 ) ;
						printf ( "mat4 type" ) ;
						texColor ( 7 ) ;
					}break;
				}

				if ( uniform_type != u_UNDEFINED ) {
					
					// leaf

					std::vector<std::string> names ;

					names.push_back ( word ) ;

					Uniform *root = p ;

					while ( root ) {
						names.push_back ( root->get_address ( ) ) ;
						root = root->get_parent ( ) ;
					}

					std::string cname ;

					for ( int i = names.size ( ) - 1 ; i + 1 ; i-- ) {

						cname += names [ i ] ;

					}

					uniform->set_location ( glGetUniformLocation ( program , cname.c_str ( ) ) ) ;
				}

				texColor ( 7 ) ;
				printf ( " and parent =" ) ;
				texColor ( 14 ) ;
				printf ( " \"%s\" .\n" , p->get_address ( ).c_str ( ) ) ;
				texColor ( 7 ) ;

				p->push_child ( word , uniform ) ;

				this->uniforms [ word ] = uniform ;
				arr.push_back ( uniform ) ;
				// add one to each of the previous uniforms
			}
		}

		ustack.push ( arr ) ;

	}

	if ( 0 || !ustack.empty ( ) ) {
		texColor ( 12 ) ;
		printf ( "SYNTAX ERROR>" ) ;
		texColor ( 14 ) ;
		printf ( "FILE " ) ;
		texColor ( 10 ) ;
		printf ( "%s\n" , fconfig.c_str ( ) ) ;
		texColor ( 7 ) ;
	}

	fclose ( conf ) ;
}

int Shader::get_location ( const std::string &name ) const {
	return glGetUniformLocation ( program , name.c_str ( ) ) ;
}

void Shader::bind ( void ) {
	glUseProgram ( program ) ;
}
void Shader::unbind ( void ) {
	glUseProgram ( 0 ) ;
}

#define DEBUG				0

void Shader::uniform ( const char *name , bool data ) const {
	if ( DEBUG ) printf ( "<Shader::uniform::%s> adressed by program - bool\n" , name );
	uniform ( glGetUniformLocation ( program , name ) , data );
}
void Shader::uniform ( const char *name , int data ) const {
	if ( DEBUG ) printf ( "<Shader::uniform::%s> adressed by program - int\n" , name );
	uniform ( glGetUniformLocation ( program , name ) , data );
}
void Shader::uniform ( const char *name , float data ) const {
	if ( DEBUG ) printf ( "<Shader::uniform::%s> adressed by program - float\n" , name );
	uniform ( glGetUniformLocation ( program , name ) , data );
}
void Shader::uniform ( const char *name , glm::vec2 data ) const {
	if ( DEBUG ) printf ( "<Shader::uniform::%s> adressed by program - vec2\n" , name );
	uniform ( glGetUniformLocation ( program , name ) , data );
}
void Shader::uniform ( const char *name , glm::vec3 data ) const {
	if ( DEBUG ) printf ( "<Shader::uniform::%s> adressed by program - vec3\n" , name );
	uniform ( glGetUniformLocation ( program , name ) , data );
}
void Shader::uniform ( const char *name , glm::vec4 data ) const {
	if ( DEBUG ) printf ( "<Shader::uniform::%s> adressed by program - vec4\n" , name );
	uniform ( glGetUniformLocation ( program , name ) , data );
}
void Shader::uniform ( const char *name , glm::mat2 data ) const {
	if ( DEBUG ) printf ( "<Shader::uniform::%s> adressed by program - mat2\n" , name );
	uniform ( glGetUniformLocation ( program , name ) , data );
}
void Shader::uniform ( const char *name , glm::mat3 data ) const {
	if ( DEBUG ) printf ( "<Shader::uniform::%s> adressed by program - mat3\n" , name );
	uniform ( glGetUniformLocation ( program , name ) , data );
}
void Shader::uniform ( const char *name , glm::mat4 data ) const {
	if ( DEBUG ) printf ( "<Shader::uniform::%s> adressed by program - mat4\n" , name );
	uniform ( glGetUniformLocation ( program , name ) , data );
}


//

void Shader::uniform ( int upos , bool data ) const {
	glUniform1i ( upos , data );
}
void Shader::uniform ( int upos , int data ) const {
	glUniform1i ( upos , data );
}
void Shader::uniform ( int upos , float data ) const {
	glUniform1f ( upos , data );
}
void Shader::uniform ( int upos , glm::vec2 data ) const {
	glUniform2f ( upos , data.x , data.y );
}
void Shader::uniform ( int upos , glm::vec3 data ) const {
	glUniform3f ( upos , data.x , data.y , data.z );
}
void Shader::uniform ( int upos , glm::vec4 data ) const {
	glUniform4f ( upos , data.x , data.y , data.z , data.w );
}
void Shader::uniform ( int upos , glm::mat2 data ) const {
	glUniformMatrix2fv ( upos , 1 , GL_FALSE , &data [ 0 ][ 0 ] );
}
void Shader::uniform ( int upos , glm::mat3 data ) const {
	glUniformMatrix3fv ( upos , 1 , GL_FALSE , &data [ 0 ][ 0 ] );
}
void Shader::uniform ( int upos , glm::mat4 data ) const {
	glUniformMatrix4fv ( upos , 1 , GL_FALSE , &data [ 0 ][ 0 ] );
}
