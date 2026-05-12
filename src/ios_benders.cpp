/// @file
/// @date 2026-05-12 (last modification)
/// @brief  Implementacje dla hierarchi `ios_bender`
///         ========================================
/// @details Created by w-borkowski on 23.10.24.
///
#include "ios_benders.h"

merry_tools::streams::ios_bender::~ios_bender() {
}

merry_tools::streams::ios_teacher::ios_teacher(const char *name): name(name) {
    std::cerr<<COLOR6<<"Empty tester io "<<COLERR<<name<<COLOR6<<" constructed."
            <<NOCOLO<<std::endl;
    std::cerr.flush();
}

merry_tools::streams::ios_teacher::ios_teacher(std::ios_base &stream, const char *name): ios_bender(stream), name(name) {
    the_stream->sync_with_stdio();
    std::cerr << COLOR6 << "Tester io " << COLERR << name << COLOR6 << " for stream " << reinterpret_cast<unsigned long long>(the_stream) << " constructed."
              << NOCOLO << std::endl;
    std::cerr.flush();
}

void merry_tools::streams::ios_teacher::set(std::ios_base &stream) {
    the_stream = &stream;
    the_stream->sync_with_stdio();
    std::cerr << COLOR6 << "Tester io " << COLERR << name << COLOR6 << " for stream " << reinterpret_cast<unsigned long long>(the_stream) << " set."
              << NOCOLO << std::endl;
    std::cerr.flush();
}

merry_tools::streams::ios_teacher::~ios_teacher() {
    the_stream->sync_with_stdio();
    std::cerr << COLOR6 << "Tester io " << COLERR << name << COLOR6 << " for stream " << reinterpret_cast<unsigned long long>(the_stream) << " destroyed."
              << NOCOLO << std::endl;
    std::cerr.flush();
}
