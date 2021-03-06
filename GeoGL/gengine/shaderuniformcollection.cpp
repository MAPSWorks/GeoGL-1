
#include "shaderuniformcollection.h"
#include "shaderuniform.h"
//#include "gengine.h"

#include <iostream>
#include <string>

using namespace std;

namespace gengine {

	ShaderUniformCollection::ShaderUniformCollection(void)
	{
	}


	ShaderUniformCollection::~ShaderUniformCollection(void)
	{
		for (vector<ShaderUniform*>::iterator it=_uniforms.begin(); it!=_uniforms.end(); ++it)
			delete *it;
	}

	/// <summary>
	/// Array operator returns a uniform by name
	/// </summary>
	ShaderUniform& ShaderUniformCollection::operator[](const std::string& Name)
	{
		return GetUniform(Name);
	}

	/// <summary>
	/// Clear all the information stored.
	/// </summary>
	void ShaderUniformCollection::Clear(void)
	{
		_uniforms.clear();
	}

	/// <summary>
	/// Query the shaders for all the uniform names and locations
	/// </summary>
	/// <param name="shader_id"></param>
	void ShaderUniformCollection::BuildFromShaderProgram(int shader_id)
	{
		int total = -1;
		glGetProgramiv(shader_id, GL_ACTIVE_UNIFORMS, &total );
		for(int i=0; i<total; ++i)  {
			int name_len=-1, num=-1;
			GLenum type = GL_ZERO;
			char name[256];
			glGetActiveUniform(shader_id, GLuint(i), sizeof(name)-1, &name_len, &num, &type, name);
			name[name_len] = 0;
			GLuint location = glGetUniformLocation(shader_id, name);
			//ShaderUniform u(shader_id,std::string(name),type,location);
			//_uniforms.push_back(u);
			ShaderUniform* u;
			switch (type) {
			case GL_INT : //or GL_UNSIGNED_INT ?
				u = new ShaderUniformInt(shader_id,std::string(name),type,location);
				_uniforms.push_back(u);
				break;
			case GL_FLOAT :
				u = new ShaderUniformFloat(shader_id,std::string(name),type,location);
				_uniforms.push_back(u);
				break;
			case GL_FLOAT_VEC2 :
				u = new ShaderUniformVec2(shader_id,std::string(name),type,location);
				_uniforms.push_back(u);
				break;
			case GL_FLOAT_VEC3 :
				u = new ShaderUniformVec3(shader_id,std::string(name),type,location);
				_uniforms.push_back(u);
				break;
			case GL_FLOAT_VEC4 :
				u = new ShaderUniformVec4(shader_id,std::string(name),type,location);
				_uniforms.push_back(u);
				break;
			//you don't get these for single precision shaders
			//case ? :
			//	u = new ShaderUniformDVec3(shader_id,std::string(name),type,location);
			//	_uniforms.push_back(u);
			//	break;
			//case ? :
			//	u = new ShaderUniformDVec4(shader_id,std::string(name),type,location);
			//	_uniforms.push_back(u);
			//	break;

			//GL_FLOAT_MAT2
			//GL_FLOAT_MAT3
			case GL_FLOAT_MAT4:
				u = new ShaderUniformMat4(shader_id,std::string(name),type,location);
				_uniforms.push_back(u);
				break;
			//GL_SAMPLER_1D
			//GL_SAMPLER_3D
			case GL_SAMPLER_2D:
				u = new ShaderUniformSampler2D(shader_id,std::string(name),type,location);
				_uniforms.push_back(u);
				break;
			}
		}

	}

	/// <summary>
	/// DEBUG: write out all the shader uniforms in the collection to the console
	/// </summary>
	void ShaderUniformCollection::Debug_Enumerate()
	{
		cout<<"Shader uniforms"<<endl;
		cout<<"Index: Shader ID, Location, Name, Type Enum"<<endl;
		int i=0;
		for (vector<ShaderUniform*>::iterator it = _uniforms.begin(); it!=_uniforms.end(); ++it) {
			ShaderUniform* u = *it;
			cout<<i<<": "<<u->_shaderID<<" "<<u->_Location<<" "<<u->_Name<<" "<<u->_Type<<endl;
			++i;
		}
	}

	/// <summary>
	/// Find the index of a named uniform. Returns -1 for not found or the index.
	/// </summary>
	int ShaderUniformCollection::FindLocationIndex(const std::string& Name)
	{
		// TODO: not exactly elegant, but there aren't going to be many uniforms and a hash is probably overkill?
		for (vector<ShaderUniform*>::iterator it = _uniforms.begin(); it!=_uniforms.end(); ++it) {
			ShaderUniform* u = *it;
			if (u->_Name==Name)
				return u->_Location;
		}
		return -1; //not found
	}

	/// <summary>
	/// Return a strongly typed uniform with the given name which allows the user to set the value so that it is stored
	/// </summary>
	ShaderUniform& ShaderUniformCollection::GetUniform(const std::string& Name)
	{
		for (vector<ShaderUniform*>::iterator it = _uniforms.begin(); it!=_uniforms.end(); ++it) {
			ShaderUniform* u = *it;
			if (u->_Name==Name)
				return **it;
		}
		//return nullptr; //not found - from "return ShaderUniform*" code
		return **_uniforms.end(); //this should cause an error if you try and set it
	}

	/// <summary>
	/// Send a matrix to the shader using its name
	/// </summary>
	void ShaderUniformCollection::SetMatrix4fv(const std::string& Name, const glm::mat4& m)
	{
		int Location = FindLocationIndex(Name);
		if (Location==-1) cerr<<"Error: shader uniform \""<<Name<<"\" not found"<<endl;
		else {
			glUniformMatrix4fv(Location, 1, GL_FALSE, &m[0][0]); // Send our matrix to the shader
		}
	}

	/// <summary>
	/// Send a double precision matrix to the shader by name. At the moment this just casts the double values in the matrix to floats, but
	/// a final implementation should do something clever with the shader to implement double precision calculations.
	/// </summary>
	void ShaderUniformCollection::SetMatrix4dv(const std::string& Name, const glm::dmat4& m)
	{
		int Location = FindLocationIndex(Name);
		if (Location==-1) cerr<<"Error: shader uniform \""<<Name<<"\" not found"<<endl;
		else {
			float f[4][4];
			for (int i=0; i<4; i++) for (int j=0; j<4; j++) f[i][j]=(float)m[i][j];
			glUniformMatrix4fv(Location, 1, GL_FALSE, &f[0][0]); // Send our matrix to the shader
		}
	}

	void ShaderUniformCollection::SetUniform1i(const std::string& Name, const int i)
	{
		int Location = FindLocationIndex(Name);
		if (Location==-1) cerr<<"Error: shader uniform \""<<Name<<"\" not found"<<endl;
		else {
			glUniform1i(Location, i);
		}
	}

	void ShaderUniformCollection::SetUniform1f(const std::string& Name, const float f)
	{
		int Location = FindLocationIndex(Name);
		if (Location==-1) cerr<<"Error: shader uniform \""<<Name<<"\" not found"<<endl;
		else {
			glUniform1f(Location, f);
		}
	}

	void ShaderUniformCollection::SetUniform2fv(const std::string& Name, const glm::vec2& v)
	{
		int Location = FindLocationIndex(Name);
		if (Location==-1) cerr<<"Error: shader uniform \""<<Name<<"\" not found"<<endl;
		else {
			glUniform2fv(Location, 1, &v[0]);
		}
	}

	void ShaderUniformCollection::SetUniform3fv(const std::string& Name, const glm::vec3& v)
	{
		int Location = FindLocationIndex(Name);
		if (Location==-1) cerr<<"Error: shader uniform \""<<Name<<"\" not found"<<endl;
		else {
			glUniform3fv(Location, 1, &v[0]);
		}
	}

	void ShaderUniformCollection::SetUniform4fv(const std::string& Name, const glm::vec4& v)
	{
		int Location = FindLocationIndex(Name);
		if (Location==-1) cerr<<"Error: shader uniform \""<<Name<<"\" not found"<<endl;
		else {
			glUniform4fv(Location, 1, &v[0]);
		}
	}

	void ShaderUniformCollection::SetUniform3dv(const std::string& Name, const glm::dvec3& v)
	{
		int Location = FindLocationIndex(Name);
		if (Location==-1) cerr<<"Error: shader uniform \""<<Name<<"\" not found"<<endl;
		else {
			glm::vec3 fv(v); //cast it down to a float vector
			glUniform3fv(Location, 1, &fv[0]);
		}
	}

	void ShaderUniformCollection::SetUniform4dv(const std::string& Name, const glm::dvec4& v)
	{
		int Location = FindLocationIndex(Name);
		if (Location==-1) cerr<<"Error: shader uniform \""<<Name<<"\" not found"<<endl;
		else {
			glm::vec4 fv(v); //cast it down to a float vector
			glUniform4fv(Location, 1, &fv[0]);
		}
	}

	/// <summary>
	/// Bind all the uniforms to the shader using the currently stored values
	/// </summary>
	void ShaderUniformCollection::Bind(void)
	{
		for (vector<ShaderUniform*>::iterator it = _uniforms.begin(); it!=_uniforms.end(); ++it)
		{
			//(*it)->Bind();
			ShaderUniform* u = (*it);
			u->Bind();
		}
	}

} //namespace gengine
