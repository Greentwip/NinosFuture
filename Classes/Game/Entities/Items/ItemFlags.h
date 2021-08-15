#ifndef __GAME_ITEM_FLAGS_H__
#define __GAME_ITEM_FLAGS_H__


namespace game {
    enum ItemId {
        Life = 1,
        Helmet,
        Head,
        Chest,
        Fist,
        Boot,
        HealthSmall,
        HealthBig,
        EnergySmall,
        EnergyBig,
        eTank,
        mTank
    };

    enum HealthPower {
        HealthSmallPower = 3,
        HealthBigPower = 5
    };

    enum EnergyPower {
        EnergySmallPower = 3,
        EnergyBigPower = 5
    };
}

#endif