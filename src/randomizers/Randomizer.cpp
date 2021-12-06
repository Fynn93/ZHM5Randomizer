#include "Randomizer.h"

#include <algorithm>
#include <random>

#include "ZHM5Randomizer/src/Config.h"
#include "ZHM5Randomizer/src/Console.h"
#include "ZHM5Randomizer/src/DefaultItemPool.h"
#include "ZHM5Randomizer/src/Item.h"
#include "ZHM5Randomizer/src/Offsets.h"
#include "ZHM5Randomizer/src/RNG.h"
#include "ZHM5Randomizer/src/Repository.h"

namespace hitman_randomizer {

RandomizationStrategy::RandomizationStrategy(
    std::shared_ptr<Config> config, std::shared_ptr<RandomDrawRepository> repo)
    : repo_(repo), config_(config) {}

void RandomizationStrategy::initialize(Scenario, const DefaultItemPool *const) {}

const RepositoryID *
UnlimitedNPCRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo_->contains(*in_out_ID)) {
    log::info(
        "DefaultNPCRandomization::randomize: skipped (not in repo) [{}]",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo_->getItem(*in_out_ID);

  // Only NPC weapons are randomized here, return original item if item isn't a
  // weapon
  if (!in_item->isWeapon()) {
    log::info(
        "DefaultNPCRandomization::randomize: skipped (not a weapon) [{}]",
        repo_->getItem(*in_out_ID)->string().c_str());
    return in_out_ID;
  }

  auto randomized_item = repo_->getRandom(&Item::isWeapon);
  return randomized_item;
}

// TODO: factor this fn
const RepositoryID *
DefaultNPCRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo_->contains(*in_out_ID)) {
    log::info(
        "DefaultNPCRandomization::randomize: skipped (not in repo) [{}]",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo_->getItem(*in_out_ID);

  // Only NPC weapons are randomized here, return original item if item isn't a
  // weapon
  if (!in_item->isWeapon()) {
    // log::info(
    //     "DefaultNPCRandomization::randomize: skipped (not a weapon) [{}]",
    //     repo_->getItem(*in_out_ID)->string().c_str());
    return in_out_ID;
  }

  auto sameType = [&in_item](const Item &item) {
    return in_item->getType() == item.getType() &&
    (item.isItemAcceptableDefaultItem() || item.isWeapon());
  };

  auto randomized_item = repo_->getRandom(sameType);
  log::info("DefaultNPCRandomization::randomize: {} -> {} ({})",
               repo_->getItem(*in_out_ID)->string().c_str(),
               repo_->getItem(*randomized_item)->string().c_str(),
               repo_->getItem(*randomized_item)->GetId().c_str());

  return randomized_item;
}

const RepositoryID *
DefaultHeroRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo_->contains(*in_out_ID)) {
    log::info(
        "DefaultHeroRandomization::randomize: skipped (not in repo) [{}]",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo_->getItem(*in_out_ID);

  auto sameType = [&in_item](const Item &item) {
    return in_item->getType() == item.getType() &&
    (item.isItemAcceptableDefaultItem() || item.isWeapon());
  };

  auto randomized_item = repo_->getRandom(sameType);
  log::info("DefaultHeroRandomization::randomize: {} -> {}",
               repo_->getItem(*in_out_ID)->string().c_str(),
               repo_->getItem(*randomized_item)->string().c_str());

  return randomized_item;
};

const RepositoryID *
DefaultStashRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo_->contains(*in_out_ID)) {
    log::info(
        "DefaultStashRandomization::randomize: skipped (not in repo) [{}]",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo_->getItem(*in_out_ID);

  auto sameType = [&in_item](const Item &item) {
    return in_item->getType() == item.getType() && 
      (item.isItemAcceptableDefaultItem() || item.isWeapon());
  };

  auto randomized_item = repo_->getRandom(sameType);
  log::info("DefaultStashRandomization::randomize: {} -> {}",
               repo_->getItem(*in_out_ID)->string().c_str(),
               repo_->getItem(*randomized_item)->string().c_str());

  return randomized_item;
};

Randomizer::Randomizer(RandomizationStrategy *strategy_) {
  strategy = std::unique_ptr<RandomizationStrategy>(strategy_);
}

const RepositoryID *Randomizer::randomize(const RepositoryID *id) {
  // printf("{}\n", id->toString().c_str());
  if (enabled)
    return strategy->randomize(id);
  else
    return id;
}

void Randomizer::initialize(Scenario scen,
                            const DefaultItemPool *const default_pool) {
  enabled = true;
  strategy->initialize(scen, default_pool);
}

void Randomizer::disable() { enabled = false; }

const RepositoryID *
IdentityRandomization::randomize(const RepositoryID *in_out_ID) {
  return in_out_ID;
}

const RepositoryID *
HardNPCRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo_->contains(*in_out_ID)) {
    log::info(
        "DefaultNPCRandomization::randomize: skipped (not in repo) [{}]",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo_->getItem(*in_out_ID);

  // flash grenades -> frag grenades
  if ((*in_out_ID == RepositoryID("042fae7b-fe9e-4a83-ac7b-5c914a71b2ca"))) {
    return repo_->getStablePointer(
        RepositoryID("3f9cf03f-b84f-4419-b831-4704cff9775c"));
  }

  // Only NPC weapons are randomized here, return original item if item isn't a
  // weapon
  if (!in_item->isWeapon()) {
    log::info(
        "DefaultNPCRandomization::randomize: skipped (not a weapon) [{}]",
        repo_->getItem(*in_out_ID)->string().c_str());
    return in_out_ID;
  }

  auto shotgun = repo_->getStablePointer(
      RepositoryID("901a3b51-51a0-4236-bdf2-23d20696b358"));
  auto rifle = repo_->getStablePointer(
      RepositoryID("d8aa6eba-0cb7-4ed4-ab99-975f2793d731"));
  auto sniper = repo_->getStablePointer(
      RepositoryID("43d15bea-d282-4a91-b625-8b7ba85c0ad5"));
  auto pistol = repo_->getStablePointer(
      RepositoryID("304fd49f-0624-4691-8506-149a4b16808e"));
  auto smg = repo_->getStablePointer(
      RepositoryID("e206ed81-0559-4289-9fec-e6a3e9d4ee7c"));

  if (in_item->isPistol()) {
    int i = rand() % 10;
    if (i == 0) {
      return pistol;
    } else {
      return smg;
    }
  }

  int i = rand() % 100;
  if (i >= 0 && i < 45) {
    return shotgun;
  } else if (i >= 45 && i < 90) {
    return rand() % 2 == 0 ? smg : rifle;
  } else {
    return sniper;
  }
}

const RepositoryID *
SleepyNPCRandomization::randomize(const RepositoryID *in_out_ID) {
  if (!repo_->contains(*in_out_ID)) {
    log::info(
        "SleepyNPCRandomization::randomize: skipped (not in repo) [{}]",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo_->getItem(*in_out_ID);

  if (!in_item->isWeapon()) {
    return in_out_ID;
  }

  return repo_->getStablePointer(
      RepositoryID("6c3854f6-dbe0-410c-bd01-ddc35b402d0c"));
}

const RepositoryID *
ChainReactionNPCRandomization::randomize(const RepositoryID *in_out_ID) {
  auto shotgun1 = repo_->getStablePointer(
      RepositoryID("0af419f5-e6d3-488d-b133-6ce0964b0770"));
  auto shotgun2 = repo_->getStablePointer(
      RepositoryID("d5728a0f-fe8d-4e2d-9350-03cf4243c98e"));
  auto rifle1 = repo_->getStablePointer(
      RepositoryID("6e4afb04-417e-4cfc-aaa2-43f3ecca9037"));
  auto rifle2 = repo_->getStablePointer(
      RepositoryID("e206ed81-0559-4289-9fec-e6a3e9d4ee7c"));
  auto sniper = repo_->getStablePointer(
      RepositoryID("370580fc-7fcf-47f8-b994-cebd279f69f9"));

  if (!repo_->contains(*in_out_ID)) {
    log::info(
        "ChainReactionNPCRandomization::randomize: skipped (not in repo) [{}]",
        in_out_ID->toString().c_str());
    return in_out_ID;
  }

  auto in_item = repo_->getItem(*in_out_ID);

  if (!in_item->isWeapon()) {
    return in_out_ID;
  }

  // flash grenades -> octane booster
  if ((*in_out_ID == RepositoryID("042fae7b-fe9e-4a83-ac7b-5c914a71b2ca")))
    return repo_->getStablePointer(
        RepositoryID("c82fefa7-febe-46c8-90ec-c945fbef0cb4"));

  int i = rand() % 10;
  if (i >= 0 && i < 4) {
    // Cure coin
    return repo_->getStablePointer(
        RepositoryID("6c3854f6-dbe0-410c-bd01-ddc35b402d0c"));
  } else if (i >= 4 && i < 8) {
    // Octane booster
    return repo_->getStablePointer(
        RepositoryID("c82fefa7-febe-46c8-90ec-c945fbef0cb4"));
  } else {
    int i = rand() % 100;
    if (i >= 0 && i < 40) {
      return rand() % 2 == 0 ? shotgun1 : shotgun2;
    } else if (i >= 40 && i < 80) {
      return rand() % 2 == 0 ? rifle1 : rifle2;
    } else {
      return sniper;
    }
  }
}

}  // namespace hitman_randomizer