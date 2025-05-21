#include "Application.h"


int main(int argc, char** argv) {
	auto& app = Application::Get();
	app.Run();
}