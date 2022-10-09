#include "FoolWindow.h"
#include "../d3d_backend/ImGuiD3D11.h"
#include "../include/httplib.h"

bool FoolWindow::render()
{
    if (show_window) {
        ImGui::Begin("fool", &show_window);

        if (ImGui::Button(u8"开始记录")) {
            //task = std::thread([]() { system("python python/scripts/record.py"); }); 
            //task.detach();
            
            // HTTP
            httplib::Client cli("http://localhost:8383");

            auto res = cli.Get("/start_record");
            std::cout << "start_record: " << res->status;
            std::cout << "start_record: body" << res->body;
        }

        if (ImGui::Button(u8"停止记录")) {
            httplib::Client cli("http://localhost:8383");

            auto res = cli.Get("/stop_record");
            std::cout << "stop_record: " << res->status;
            std::cout << "stop_record: body" << res->body;
        }
        //ImGui::Image((ImTextureID)d3d11SRV, ImVec2(captureWidth, captureHeight));

        //ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)d3d11SRV, ImVec2(0, 0), ImVec2(captureWidth, captureHeight));

        ImGui::End();
    }


    return false;
}

