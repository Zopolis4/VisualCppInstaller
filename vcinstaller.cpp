// This program reads the file visualstudio.vsman, produces a list of available packages
// and generate bat script that one can execute to download and install the packages

#define VS_PKGS_MAN_FILE "pkgs.json"

#include <stdio.h>
//#include <fstream>

#if 1

#include "json.hpp"

using json = nlohmann::json;

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

#endif

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

	// Alternative way to read JSON; but this does not display error for debugging
	/*std::ifstream input_stream("visualstudio.vsman");
	json j;
	input_stream >> j;*/
	auto j = json::parse(text);

	// Stage 1: Use this to detect the structure of the file
	print_type(j, 1);

	// Stage 2: List the available packages by id to know what to get
	auto pkgs = j["packages"];
	auto numpkgs = pkgs.size();
	printf("\nNumber of packages: %d\n", numpkgs);
	// Stage 3: Generate bat script to download and install VC headers and libraries
	auto download_script = fopen("vc_download.sh", "w+b");
	auto install_script = fopen("vc_install.bat", "w+b");
	bool print_version = true;
	if (download_script == NULL || install_script == NULL)
	{
		printf("Can't create download/install script\n");
		return 1;
	}
	for(int i = 0; i < numpkgs; i++)
	{
		auto p = pkgs[i];
		std::string pid = p["id"]; // p["id"].get<std::string>()

		// Filter packages related to Visual C++
		// TODO Enhance this to take a list of packages as arguments
		// TODO Filter out the Resources packages
		if (!strncmp(pid.c_str(), "Microsoft.VisualC.", 18))
		{
			//Stage 2: `std::cout << p["id"] << " : " << p["chip"] <<  "\n";`
			auto payloads = p["payloads"];
			auto pids = pid.c_str();
			if (print_version)
			{
				std::cout << "VC version: " << p["version"] << "\n";
				print_version = false;
			}

			// mkdir for the package (package can share payload file such as cab1.cab)
			// that's why we need a separate folder for each package
			// then cd into the directory to run wget there
			fprintf(download_script, "test -e %s || mkdir %s\ncd %s\n", pids, pids, pids);
			std::string msiFileName;
			bool hasMsi = false;
			for(int k = 0; k < payloads.size(); k++)
			{
				std::string fileName = payloads[k]["fileName"];
				std::string url = payloads[k]["url"];
				fprintf(download_script, "wget --no-check-certificate %s\n", url.c_str());
				if (!strstr(fileName.c_str(), ".Msi"))
				{
					msiFile = fileName;
					hasMsi = true;
				}
			}
			fprintf(download_script, "cd ..\n");

			// Use msiexec to deploy the package
			if (hasMsi)
			{
				fprintf(install_script, "cd %s && msiexec /a \"%s\" TARGETDIR=%%VCINSTALLDIR%% && cd ..\n", pids, msiFileName);
			}
		}
	}
	fclose(download_script);
	fclose(install_script);

	return 0;
}