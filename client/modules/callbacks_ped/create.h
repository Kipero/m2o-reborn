void module_ped_callback_create(librg_event_t *event) {
    if (librg_entity_type(event->entity) != TYPE_PED) return;

    auto transform = librg_fetch_transform(event->entity);

    const char *directory = "/sds/traffic/";
    const char *model = "cvezjon";

    M2::Wrappers::ModelManager *pModelManager = new M2::Wrappers::ModelManager();
    if (!pModelManager) {
        return;
    }

    M2::Wrappers::GameModelManager *pPedModelManager = pModelManager->Load(directory, model);
    if (!pPedModelManager) {
        return;
    }

    M2::C_Human2 *human = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);
    if (!human) {
        return;
    }

    M2::C_Model *pModel = M2::C_Core::Get()->AllocateModel(2);
    if (!pModel) {
        return;
    }

    pModel->CloneHierarchy(pPedModelManager->GetModelManager()->m_pModel);
    if (!pModel) {
        return;
    }

    pModel->SetName("m2online_ped");
    pModel->MarkForNotify(2);

    reinterpret_cast<M2::C_Entity *>(human)->SetModel(pModel);
    reinterpret_cast<M2::C_Entity *>(human)->Setup();

    DWORD flags = reinterpret_cast<M2::C_Entity *>(human)->m_dwFlags &= 0xFFFFB7BF | 0x4800;
    reinterpret_cast<M2::C_Entity *>(human)->m_dwFlags = flags;

    if (reinterpret_cast<M2::C_Entity *>(human)->m_dwFlags & 0x20)
        mod_log("Flags set sucessfully!\n");
    else {
        reinterpret_cast<M2::C_Entity *>(human)->Release();
    }

    reinterpret_cast<M2::C_Entity *>(human)->Activate();

    if (reinterpret_cast<M2::C_Entity *>(human)->IsActive())
        mod_log("Entity active !\n");

    reinterpret_cast<M2::C_Entity *>(human)->SetPosition(transform->position);

    mod_log("Created at %x!\n", human);

    librg_attach_gamedata(event->entity, { (M2::C_Entity*)human, pPedModelManager });
}
