#ifndef BATCHRUNNER_H
#define BATCHRUNNER_H

#include "Config.h"

// 一括処理全体を実行するクラス
class BatchRunner {
public:
    bool run(const Config& config);
};

#endif