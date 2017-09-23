// This program reads the file visualstudio.vsman, produces a list of available packages
// and generate bat script that one can execute to download and install the packages

#include <stdio.h>
//#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

#define VS_PKGS_MAN_FILE "pkgs.json"

static inline void print_indent(int indent)
{
	for(int i = 0; i < indent; i++)
		printf("  ");
}

void print_type(json & j, int indent)
{
	if (j.is_null())
		printf("NULL");
	else if (j.is_boolean())
		printf("BOOL");
	else if (j.is_number())
		printf("NUM");
	else if (j.is_string())
		printf("STRING");
	else if (j.is_object())
	{
		printf("OBJ {\n");
		for (json::iterator it = j.begin(); it != j.end(); ++it)
		{
			print_indent(indent);
			std::cout << "\"" << it.key() << "\" : ";
			print_type(it.value(), indent + 1);
			printf("\n");
		}
		print_indent(indent);
		printf("}");
	}
	else if (j.is_array())
	{
		printf("ARRAY of ");
		if (j.size() > 0)
			print_type(j[0], indent);
		else printf("Unknown");
	}
}

int main(int argc, char** argv)
{
	auto file = fopen(VS_PKGS_MAN_FILE, "rb");
	if (file == NULL)
	{
		printf("File %s not found!\n", VS_PKGS_MAN_FILE);
		return 1;
	}

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	printf("Read %d bytes in %s\n", size, VS_PKGS_MAN_FILE);
	auto text = new char[size + 1];
	fread(text, sizeof(char), size, file);
	printf("Finish reading.\n");
	fclose(file);

	auto j = json::parse(text);

	// Stage 1: Use this to detect the structure of the file
	print_type(j, 1);
	
	//auto info = j["info"];
	// Stage 2: List the available packages by id
	auto pkgs = j["packages"];
	auto numpkgs = pkgs.size();
	printf("\nNumber of packages: %d\n", numpkgs);
	auto download_script = fopen("vc_download.sh", "wb");
	auto install_script = fopen("vc_install.bat", "wb");
	for(int i = 0; i < numpkgs; i++)
	{
		auto p = pkgs[i];
		std::string pid = p["id"]; // p["id"].get<std::string>()
		auto pchip = p["chip"];
		if (!strncmp(pid.c_str(), "Microsoft.VisualC", 17))
		{
			//std::cout << pid << " : " << pchip <<  "\n";
			auto pl = p["payloads"];

			// mkdir for the package (package can share payload file such as cab1.cab)
			// that's why we need a separate folder for each package
			// then cd into the directory to run wget there
			fprintf(download_script, "test -e %s || mkdir %s\ncd %s\n", pid.c_str(), pid.c_str(), pid.c_str());
			for(int k = 0; k < pl.size(); k++)
			{
				std::string url = pl[k]["url"];
				fprintf(download_script, "wget --no-certificate-check %s\n", url.c_str());
			}
			printf(download_script, "cd ..\n");

			// Use msiexec to deploy the package
			printf(install_script, "msiexec /a \"%s/%s\" TARGETDIR=%%VCINSTALLDIR%%\n", pid.c_str(), pid.c_str());
		}
	}
	fclose(download_script);
	fclose(install_script);

	// Stage 3: Generate bat script to download and install VC headers and libraries


	/*std::ifstream input_stream("visualstudio.vsman");
	json j;
	input_stream >> j;
	std::cout << j;*/

	return 0;
}