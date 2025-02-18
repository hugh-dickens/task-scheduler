#include "file_conversions/excel_handler.hpp"
#include <fstream>
#include <iostream>

bool ExcelHandler::supports(const std::string& filePath) const {
    return filePath.ends_with(".xlsx");
}

std::unique_ptr<IFileHandler> ExcelHandler::clone() const {
    return std::make_unique<ExcelHandler>();
}

// Read Excel file into a structured format
std::vector<std::vector<std::string>> ExcelHandler::read(const std::string& filePath) {
    OpenXLSX::XLDocument doc;
    std::vector<std::vector<std::string>> tableData;

    try {
        doc.open(filePath);
        OpenXLSX::XLWorksheet ws = doc.workbook().worksheet("Sheet1");

        for (auto row = 1; row <= ws.rowCount(); ++row) {
            std::vector<std::string> rowData;
            for (auto col = 1; col <= ws.columnCount(); ++col) {
                auto cell = ws.cell(row, col);
                std::string cellValue;

                switch (cell.value().type()) {
                    case OpenXLSX::XLValueType::String:
                        cellValue = cell.value().get<std::string>();
                        break;
                    case OpenXLSX::XLValueType::Integer:
                        cellValue = std::to_string(cell.value().get<int>());
                        break;
                    case OpenXLSX::XLValueType::Float:
                        cellValue = std::to_string(cell.value().get<double>());
                        break;
                    case OpenXLSX::XLValueType::Empty:
                        cellValue = "";
                        break;
                    default:
                        cellValue = "[UNKNOWN]";
                        break;
                }
                rowData.push_back(cellValue);
            }
            tableData.push_back(rowData);
        }

        doc.close();
        return tableData;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to read Excel file: " << e.what() << std::endl;
        return {};
    }
}

// Write structured data to an Excel file
void ExcelHandler::write(const std::string& filePath, const std::vector<std::vector<std::string>>& data) {
    if (data.empty()) {
        std::cerr << "[ERROR] No data provided for Excel conversion\n";
        return;
    }

    OpenXLSX::XLDocument doc;
    try {
        // This implementation comes from the GitHub page of OpenXLSX: https://github.com/troldal/OpenXLSX. But has a deprecated compilation
        // warning. TODO: Fix the warning.
        doc.create(filePath);
        doc.workbook().addWorksheet("Sheet1");
        auto ws = doc.workbook().worksheet("Sheet1");

        // Write headers
        for (size_t col = 0; col < data[0].size(); ++col) {
            ws.cell(1, col + 1).value() = data[0][col];
        }

        // Write data
        for (size_t row = 1; row < data.size(); ++row) {
            for (size_t col = 0; col < data[row].size(); ++col) {
                ws.cell(row + 1, col + 1).value() = data[row][col];
            }
        }

        doc.save();
        doc.close();
        std::cout << "[INFO] Successfully wrote Excel file: " << filePath << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to write Excel file: " << e.what() << std::endl;
    }
}

