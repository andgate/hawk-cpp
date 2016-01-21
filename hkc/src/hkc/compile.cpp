#include "hkc/compile.h"


#include <iostream>
#include <vector>
#include <cstdlib>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;


void hkc::Compiler::run()
{
	// ----------------
	// Compiler Process
	// ----------------
    fs::path build_folder("build");
    
    // 1. Parse execution units
    std::vector<nmodule> modules;
    for(const std::string& in_file : build->in_files)
    {
        modules.push_back(parse(in_file));
    }
    
    // 2. Perform various compiler passes and checks
    for(auto& module : modules)
    {
        //NodePrinter printer;
        //block->accept(printer);
    }
    
    
    std::vector<std::string> ir_files;
    
    fs::path ir_dir(build->build_dir);
    ir_dir /= "intermediates/ir";
    fs::create_directories(ir_dir);
    
    for(auto& module : modules)
    {
        auto out_path = ir_dir / module->name;
        out_path.replace_extension("ll");
        const std::string out_file(out_path.string());
        
        gen_ir(module, out_file);
        ir_files.push_back(std::move(out_file));
    }
    modules.clear();
    
    
    fs::path asm_dir(build->build_dir);
    asm_dir /= "intermediates/asm";
    fs::create_directories(asm_dir);
    
    std::vector<std::string> asm_files;
    for(auto& ir_file : ir_files)
    {
        auto out_path = asm_dir / fs::path(ir_file).filename();
        out_path.replace_extension("S");
        const std::string out_file(out_path.string());
        
        gen_asm(ir_file, out_file);
        asm_files.push_back(std::move(out_file));
    }
    ir_files.clear();
    
    fs::path obj_dir(build->build_dir);
    obj_dir /= "intermediates/obj";
    fs::create_directories(obj_dir);
    
    std::vector<std::string> obj_files;
    for(auto& asm_file : asm_files)
    {
        auto out_path = obj_dir / fs::path(asm_file).filename();
        out_path.replace_extension(".o");
        const std::string out_file(out_path.string());
        
        gen_obj(asm_file, out_file);
        obj_files.push_back(std::move(out_file));
    }
    asm_files.clear();
    
    link(obj_files);
    obj_files.clear();
}

nmodule hkc::Compiler::parse(const std::string& in_file)
{
    hawk_driver driver;
    //driver.trace_parsing = true;
    driver.trace_scanning = false;
    
    if(driver.parse(in_file))
    {
        cout << "Error building " << in_file << endl;
        exit(0);
    }
    
    fs::path in_path(in_file);
    std::string module_name(in_path.filename().string());
    
    auto module = std::make_shared<NModule>(module_name, driver.result);
    
    return module;
}

void hkc::Compiler::gen_ir(const nmodule& module, const std::string& out_file)
{
    CodeGen gen;
    gen.build_module(*module);
    //gen.print_ir();
    gen.write_ir(out_file);

}

void hkc::Compiler::gen_asm(const std::string& in_file, const std::string& out_file)
{
    std::string command("llc --filetype=asm " + in_file + " -o " + out_file);
    std::cout << command << std::endl;
    system(command.c_str());
}

void hkc::Compiler::gen_obj(const std::string& in_file, const std::string& out_file)
{
    std::string command("gcc -c " + in_file + " -o " + out_file);
    std::cout << command << std::endl;
    system(command.c_str());
}

void hkc::Compiler::link(const std::vector<std::string>& in_files)
{
    std::string obj_files;
    for(auto in_file : in_files)
    {
        obj_files += in_file + " ";
    }
    
    fs::path out_dir(build->build_dir);
    out_dir /= "bin";
    fs::create_directories(out_dir);
    out_dir /= build->out_file;
    
    std::string command("gcc -nostartfiles " + obj_files + "-o " + out_dir.string());
    
    if(build->debug == true) command += " -g";
    
    std::cout << command << std::endl;
    system(command.c_str());
}