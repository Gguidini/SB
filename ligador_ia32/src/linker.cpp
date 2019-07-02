#include <../lib/elfio/elfio.hpp>

class Linker{

  std::pair < char* , char* > __binary_code;
  std::string __out_name;
  bool __done;

  public:

    // Constructors 
    Linker();
    Linker(std::pair < char* , char* > binary_code);

    // Run
    bool run();
}


// Nada é inicializado como válido.
Linker::Linker(){
    __done = false;
}

// Linker com arquivo de referência.
// Arquivo de referência já é aberto.
Linker::Linker(std::pair < char* , char* > binary_code, std::string output_name){
    __binary_code = binary_code;
    __out_name = output_name;
    __done = false;
}

bool Linker::run(){

    ELFIO::elfio writer;
    
    // You can't proceed without this function call!
    writer.create( ELFCLASS32, ELFDATA2LSB );

    writer.set_os_abi( ELFOSABI_LINUX );     // OS dependent
    writer.set_type( ET_EXEC );
    writer.set_machine( EM_386 );

    // Create code section
    section* text_sec = writer.sections.add( ".text" );
    text_sec->set_type( SHT_PROGBITS );
    text_sec->set_flags( SHF_ALLOC | SHF_EXECINSTR );
    text_sec->set_addr_align( 0x10 );
    

    text_sec->set_data( __binary_code.first , sizeof( __binary_code.first  ) );

    // Create a loadable segment
    segment* text_seg = writer.segments.add();
    text_seg->set_type( PT_LOAD );
    text_seg->set_virtual_address( 0x08048000 );    // OS dependent
    text_seg->set_physical_address( 0x08048000 );   // OS dependent
    text_seg->set_flags( PF_X | PF_R );
    text_seg->set_align( 0x1000 );
    
    // Add code section into program segment
    text_seg->add_section_index( text_sec->get_index(), text_sec->get_addr_align() );

    // Create data section*
    section* data_sec = writer.sections.add( ".data" );
    data_sec->set_type( SHT_PROGBITS );
    data_sec->set_flags( SHF_ALLOC | SHF_WRITE );
    data_sec->set_addr_align( 0x4 );

    data_sec->set_data( __binary_code.second, sizeof( __binary_code.second ) );

    // Create a read/write segment
    segment* data_seg = writer.segments.add();
    data_seg->set_type( PT_LOAD );
    data_seg->set_virtual_address( 0x08048020 );    // OS dependent
    data_seg->set_physical_address( 0x08048020 );   // OS dependent
    data_seg->set_flags( PF_W | PF_R );
    data_seg->set_align( 0x10 );                    // OS dependent

    // Add code section into program segment
    data_seg->add_section_index( data_sec->get_index(), data_sec->get_addr_align() );

    section* note_sec = writer.sections.add( ".note" );
    note_sec->set_type( SHT_NOTE );
    note_sec->set_addr_align( 1 );
    note_section_accessor note_writer( writer, note_sec );
    note_writer.add_note( 0x01, "Created by ELFIO", 0, 0 );
    char descr[6] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36};
    note_writer.add_note( 0x01, "Never easier!", descr, sizeof( descr ) );

    // Setup entry point
    writer.set_entry( 0x08048000 );          // OS dependent

    // Create ELF file
    writer.save( __out_name );

    return true;
}
