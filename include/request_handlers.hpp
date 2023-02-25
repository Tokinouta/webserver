#ifndef REQUEST_HANDLERS_HPP
#define REQUEST_HANDLERS_HPP

#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <string>

#include "http_parser.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "http_states.hpp"

HttpResponse h(const std::string& url);

#endif