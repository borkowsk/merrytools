/// @file
/// @brief IOS bender to magik standardowych strumieni.
/// @date 2024-10-31 (last modification)
/// To bogato rozrodzona rodzinka pochodząca od ojca: `ios_bender`.
/// Dzieci to `text_at_end`, `keep_io_flags`, `ios_teacher` a kolejne w drodze 👅.
/// Więcej możliwości:
/// *   https://dev.to/pauljlucas/custom-c-stream-manipulators-nil
/// *   https://github.com/mathbunnyru/funny-cpp
/// Created by w-borkowski on 23.10.24 wieczorem.
///
#ifndef IOS_BENDERS_H
#define IOS_BENDERS_H
#include <iomanip>
#include <iostream>

namespace merry_tools::iostreams {

    // ESC sequences defining colors on an ANSI terminal:
//*//////////////////////////////////////////////////
// \033 is the octal ESC character code. See: "https://en.wikipedia.org/wiki/ANSI_escape_code"
    [[maybe_unused]]
    const std::string  NOCOLO="\033[0m";                 ///< Back to default color
//[[maybe_unused]]
//const std::string  NORMCO="\033[0m";                 ///< Back to default color
    [[maybe_unused]]
    const std::string  COLOR7="\033[37m";                ///< DEFAULT WHITE
    [[maybe_unused]]
    const std::string  COLOR6="\033[36m";                ///< CYAN
    [[maybe_unused]]
    const std::string  COLOR5="\033[35m";                ///< MAGENTA
    [[maybe_unused]]
    const std::string  COLOR4="\033[34m";                ///< BLUE
    [[maybe_unused]]
    const std::string  COLOR3="\033[33m";                ///< YELLOW
    [[maybe_unused]]
    const std::string  COLOR2="\033[32m";                ///< GREEN
    [[maybe_unused]]
    const std::string  COLOR1="\033[31m";                ///< RED
    [[maybe_unused]]
    const std::string  COLOR0="\033[30m";                ///< DEFAULT BLACK
    [[maybe_unused]]
    const std::string  COLERR="\033[31m";                ///< RED as error marker
    [[maybe_unused]]
    const std::string  ERCOLO="\033[38;2;255;100;100m";  ///< Set LIGHT RED foreground color 	Next arguments are 5;nm or 2;r;g;bm
    [[maybe_unused]]
    const std::string  COLFIL="\033[38;2;90;90;128m";    ///< Set DARK BLUEISH foreground color 	Next arguments are 5;nm or 2;r;g;bm
    [[maybe_unused]]
    const std::string  COLLIG="\033[38;2;200;200;255m";  ///< Set LIGHT BLUEISH foreground color 	Next arguments are 5;nm or 2;r;g;bm
    [[maybe_unused]]
    const std::string  COLBRI="\033[38;2;255;200;255m";  ///< Set VERY WHITE foreground color 	Next arguments are 5;nm or 2;r;g;bm
    [[maybe_unused]]
    const std::string  COLRET="\033[38;2;255;128;128m";  ///< Set ORANGE foreground color 	Next arguments are 5;nm or 2;r;g;bm

    /** Bazowa klasa dla obiektów manipulujących strumieniami. */
    class ios_bender {
    protected:
        std::ios_base* the_stream=nullptr;
    public:
        ios_bender() {}
        explicit ios_bender(std::ios_base& stream) : the_stream(&stream) {}
        /// Destruktor wirtualny MUSI zostać zdefiniowany u potomstwa (choć ta klasa ma go faktycznie, bo mieć musi 🤔).
        virtual ~ios_bender()=0;
        /// Sprawdza, czy wszystko ustawione i nie trzeba wywoływać `set() `.
        virtual bool is_set() const { return the_stream != nullptr; }
        /// Wywoływane w operatorze `<<` jeśli `is_set() ` zwróci `false`.
        virtual void set(std::ios_base& stream) { the_stream = &stream; }
    };

    /// Przeciążenie operatora "<<" dla jawnie modyfikowalnych "benderów".
    inline std::ostream& operator << (std::ostream& os, ios_bender& io) {
        if(!io.is_set()) { io.set(os);}
        return os;
    }

    /// Przeciążenie operatora "<<" dla teoretycznie nie modyfikowalnych "benderów".
    /** @note W rzeczywistości muszą być modyfikowane, przynajmniej wtedy, gdy nie mają ustawionego strumienia, ale też
     *        w innych sytuacjach (choćby zmienne tymczasowe wstrzykiwane w strumień z pomocą konstruktora są `const`).
     *        Dlatego wewnątrz tego operatora `const` jest zdejmowane. */
    inline std::ostream& operator << (std::ostream& os, const ios_bender& io) {
        auto& tmp=const_cast<ios_bender&>(io); //Ups! Musimy zdjąć `const` żeby cokolwiek zmienić!
        if(!tmp.is_set()) tmp.set(os);
        return os;
    }

    /// Klasa służy do testowania momentów, w których są wykonywane operacje na prawdziwie użytecznych klasach rodziny.
    /** Poza destrukcją jest to miejsce pojawienia się lub wstrzyknięcia w strumień, ale destrukcja może być wykonywana
     *  w różnych momentach:
     *  - Gdy obiekt jest nazwany, jego destrukcja odbywa się po prostu na końcu bloku, w którym jest dostępny.
     *  - Gdy jest to obiekt nienazwany, czyli użyty z pomocą konstruktora obecnego w ciągu instrukcji "<<" to jego
     *    destrukcja może się odbyć na końcu bloku lub wcześniej. Np. zakończeniu wyrażenia, w którym jest użyty, lub na
     *    końcu ciągu instrukcji. */
    class ios_teacher final : public ios_bender {
        const char* name="";  //!< "Imię" bendera, żeby można było rozróżnić ich komunikaty.
    public:
        /// Pokazuje moment skonstruowania na zsynchronizowanym z danym strumieniem `std::cerr`.
        explicit ios_teacher(const char* name="");

        /// Pokazuje moment skonstruowania. (`std::cerr` jak wyżej)
        explicit ios_teacher(std::ios_base& stream, const char* name="");

        /// Pokazuje moment przypisania wartości. (`std::cerr` jak wyżej)
        void set(std::ios_base& stream) override;

        /// Pokazuje moment, w którym ulega zniszczeniu.  (`std::cerr` jak wyżej)
        ~ios_teacher() override;
    };

    /// Klasa wrzucająca dowolny tekst na dany strumień w momencie destrukcji obiektu.
    class text_at_end: public ios_bender {

        const char* the_text="\n"; //!< Tekst do wrzucenia na strumień.

    public:
        /// Można zmienić domyślny tekst.
        explicit text_at_end(const char* end_text="\n"): the_text(end_text) {}

        /// Można ustalić strumień i domyślny tekst.
        explicit text_at_end(std::ostream& stream, const char* end_text="") : ios_bender(stream), the_text(end_text) {}

        /// Zanim ulegnie zniszczeniu, wrzuca tekst na strumień.
        ~text_at_end() override {
            std::ostream& os = *dynamic_cast<std::ostream*>(the_stream); //Zakładamy, że nikt nie wrzuca tekstu na strumień WEJŚCIOWY :-D
            os << the_text;
        }

    };

    /// A class that preserves the state of the flags of a given stream and restores them upon its destruction.
    class keep_io_flags: public ios_bender {

        std::_Ios_Fmtflags the_flags=std::_Ios_Fmtflags::_S_ios_fmtflags_min;   //!< all flags.

    public:
        /// Zarówno strumień, jak i flagi pozostają do ustawienia w `set()`
        explicit keep_io_flags() = default;

        /// Zapamiętuje pełen stan flag strumienia...
        explicit keep_io_flags(std::ios_base& stream) : ios_bender(stream), the_flags(the_stream->flags()) {}

        void set(std::ios_base& stream) override { ios_bender::set(stream); the_flags=the_stream->flags(); }

        /// Odtwarza stan flag strumienia, gdy wychodzi z zasięgu. Zazwyczaj na końcu bloku.
        ~keep_io_flags() override { if(is_set()) the_stream->flags(the_flags); }

    };

}

#endif //IOS_BENDERS_H
