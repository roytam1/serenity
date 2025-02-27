/*
 * Copyright (c) 2021, Itamar S. <itamar8910@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/FlyString.h>
#include <AK/Function.h>
#include <AK/HashMap.h>
#include <AK/Optional.h>
#include <AK/String.h>
#include <AK/StringView.h>
#include <AK/Vector.h>
#include <LibCpp/Token.h>

namespace Cpp {

class Preprocessor {

public:
    explicit Preprocessor(const String& filename, const StringView& program);
    Vector<Token> process_and_lex();
    Vector<StringView> included_paths() const { return m_included_paths; }

    struct Definition {
        String key;
        Vector<String> parameters;
        String value;
        FlyString filename;
        size_t line { 0 };
        size_t column { 0 };
    };
    using Definitions = HashMap<String, Definition>;

    struct Substitution {
        Vector<Token> original_tokens;
        Definition defined_value;
        String processed_value;
    };

    Definitions const& definitions() const { return m_definitions; }
    Vector<Substitution> const& substitutions() const { return m_substitutions; }

    void set_ignore_unsupported_keywords(bool ignore) { m_options.ignore_unsupported_keywords = ignore; }
    void set_keep_include_statements(bool keep) { m_options.keep_include_statements = keep; }

    Function<Definitions(StringView)> definitions_in_header_callback { nullptr };

private:
    using PreprocessorKeyword = StringView;
    PreprocessorKeyword handle_preprocessor_line(StringView const&);
    void handle_preprocessor_keyword(StringView const& keyword, GenericLexer& line_lexer);
    void process_line(StringView const& line);
    size_t do_substitution(Vector<Token> const& tokens, size_t token_index, Definition const&);
    Optional<Definition> create_definition(StringView line);

    struct MacroCall {
        Token name;
        struct Argument {
            Vector<Token> tokens;
        };
        Vector<Argument> arguments;
        size_t end_token_index { 0 };
    };
    Optional<MacroCall> parse_macro_call(Vector<Token> const& tokens, size_t token_index);
    String evaluate_macro_call(MacroCall const&, Definition const&);

    String m_filename;
    String m_program;
    Vector<StringView> m_lines;

    Vector<Token> m_tokens;
    Definitions m_definitions;
    Vector<Substitution> m_substitutions;

    size_t m_line_index { 0 };
    size_t m_current_depth { 0 };
    Vector<size_t> m_depths_of_taken_branches;
    Vector<size_t> m_depths_of_not_taken_branches;

    enum class State {
        Normal,
        SkipIfBranch,
        SkipElseBranch
    };
    State m_state { State::Normal };

    Vector<StringView> m_included_paths;

    struct Options {
        bool ignore_unsupported_keywords { false };
        bool keep_include_statements { false };
    } m_options;
};
}
