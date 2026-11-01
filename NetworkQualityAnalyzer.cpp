#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct Measurement {
    int number;
    bool success;
    double responseMs;
};

bool parseNumber(const std::string& input, double& value) {
    std::istringstream stream(input);
    stream >> value;
    return stream && stream.peek() == std::char_traits<char>::eof();
}

void printMeasurements(const std::vector<Measurement>& measurements) {
    std::cout << "\n--- 測定結果 ---\n";
    std::cout << std::left << std::setw(10) << "回数"
              << std::setw(15) << "結果" << "応答時間(ms)\n";
    std::cout << std::string(38, '-') << '\n';

    for (const auto& item : measurements) {
        std::cout << std::left << std::setw(10) << item.number
                  << std::setw(15) << (item.success ? "成功" : "応答なし");
        if (item.success) {
            std::cout << std::fixed << std::setprecision(2) << item.responseMs;
        } else {
            std::cout << "-";
        }
        std::cout << '\n';
    }
}

void printSummary(const std::vector<Measurement>& measurements) {
    int successes = 0;
    int failures = 0;
    double minimum = std::numeric_limits<double>::max();
    double maximum = 0.0;
    double total = 0.0;
    std::vector<double> successfulTimes;

    for (const auto& item : measurements) {
        if (!item.success) {
            ++failures;
            continue;
        }
        ++successes;
        total += item.responseMs;
        minimum = std::min(minimum, item.responseMs);
        maximum = std::max(maximum, item.responseMs);
        successfulTimes.push_back(item.responseMs);
    }

    const int count = static_cast<int>(measurements.size());
    const double successRate = count == 0 ? 0.0 : 100.0 * successes / count;
    const double lossRate = count == 0 ? 0.0 : 100.0 * failures / count;

    std::cout << "\n--- 集計 ---\n";
    std::cout << "測定回数       : " << count << '\n';
    std::cout << "成功           : " << successes << '\n';
    std::cout << "応答なし       : " << failures << '\n';
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "成功率         : " << successRate << "%\n";
    std::cout << "応答なし率     : " << lossRate << "%\n";

    if (successes == 0) {
        std::cout << "応答時間       : 成功した測定がないため計算できません\n";
        return;
    }

    const double average = total / successes;
    double squaredDifferenceSum = 0.0;
    for (double time : successfulTimes) {
        const double difference = time - average;
        squaredDifferenceSum += difference * difference;
    }
    const double standardDeviation = std::sqrt(squaredDifferenceSum / successes);

    std::cout << "最短応答時間   : " << minimum << " ms\n";
    std::cout << "最長応答時間   : " << maximum << " ms\n";
    std::cout << "平均応答時間   : " << average << " ms\n";
    std::cout << "応答時間の標準偏差: " << standardDeviation << " ms\n";

    // Compare adjacent successful measurements only; a failed trial breaks the sequence.
    double differenceTotal = 0.0;
    int differenceCount = 0;
    for (std::size_t i = 1; i < measurements.size(); ++i) {
        if (measurements[i - 1].success && measurements[i].success) {
            differenceTotal += std::abs(measurements[i].responseMs - measurements[i - 1].responseMs);
            ++differenceCount;
        }
    }
    if (differenceCount > 0) {
        std::cout << "隣接測定の平均変動: "
                  << differenceTotal / differenceCount << " ms\n";
    } else {
        std::cout << "隣接測定の平均変動: 比較できる連続した成功データがありません\n";
    }
}

bool saveCsv(const std::vector<Measurement>& measurements, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        return false;
    }

    file << "number,status,response_ms\n";
    for (const auto& item : measurements) {
        file << item.number << ',';
        if (item.success) {
            file << "success," << std::fixed << std::setprecision(2) << item.responseMs;
        } else {
            file << "no_response,";
        }
        file << '\n';
    }
    return static_cast<bool>(file);
}

int main() {
    std::vector<Measurement> measurements;
    std::string input;

    std::cout << "====================================\n";
    std::cout << "       通信品質分析ツール (C++)\n";
    std::cout << "====================================\n";
    std::cout << "通信テストなどで測定した応答時間を入力して分析します。\n";
    std::cout << "応答がなかった場合は -1 を入力してください。\n";
    std::cout << "終了するには q を入力してください。\n\n";

    while (true) {
        std::cout << "測定 " << (measurements.size() + 1)
                  << " 回目の応答時間 (ms / 応答なし=-1 / 終了=q): ";
        if (!std::getline(std::cin, input)) {
            break;
        }
        if (input == "q" || input == "Q") {
            break;
        }

        double responseMs = 0.0;
        if (!parseNumber(input, responseMs)) {
            std::cout << "数値または q を入力してください。\n";
            continue;
        }
        if (responseMs < 0.0 && responseMs != -1.0) {
            std::cout << "-1 または 0 以上の数値を入力してください。\n";
            continue;
        }
        if (measurements.size() >= 1000) {
            std::cout << "測定は最大1000回までです。\n";
            break;
        }

        const bool success = responseMs >= 0.0;
        measurements.push_back({static_cast<int>(measurements.size() + 1), success,
                                 success ? responseMs : 0.0});
    }

    if (measurements.empty()) {
        std::cout << "測定データがありません。終了します。\n";
        return 0;
    }

    printMeasurements(measurements);
    printSummary(measurements);

    std::cout << "\nCSVファイルに保存しますか？ (y/n): ";
    std::getline(std::cin, input);
    if (input == "y" || input == "Y") {
        std::cout << "保存ファイル名 (例: network_results.csv): ";
        std::getline(std::cin, input);
        if (input.empty()) {
            input = "network_results.csv";
        }
        if (saveCsv(measurements, input)) {
            std::cout << "保存しました: " << input << '\n';
        } else {
            std::cerr << "ファイルを保存できませんでした。保存先や権限を確認してください。\n";
            return 1;
        }
    }

    std::cout << "\n分析を終了します。\n";
    std::cout << "※このツールは入力された測定値を集計するもので、通信自体を測定するものではありません。\n";
    std::cout << "※ストリーミングの快適さは、応答時間以外に通信速度やWi-Fi環境などにも左右されます。\n";
    return 0;
}
