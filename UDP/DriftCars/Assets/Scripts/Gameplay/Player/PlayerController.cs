using UnityEngine;
using System.Collections.Generic;

public class PlayerController : MonoBehaviour
{
    [SerializeField]
	GameObject TargetCursorPrefab = null;

    GameObject targetCursor = null;
    int floorLayer;
    Car playerCar;

    // Use this for initialization
    void Start ()
    {
        floorLayer = 1 << LayerMask.NameToLayer("Floor");

        if (TargetCursorPrefab != null)
            targetCursor = Instantiate(TargetCursorPrefab);

        GameMgr.Race.OnRaceInitialized += InitPlayerCar;
    }

    void InitPlayerCar()
    {
        playerCar = GameMgr.Race.PlayerCar;
    }

    // Update is called once per frame
    void Update ()
    {
        if (GameMgr.Race.HasStarted == false || playerCar == null)
            return;

		if (Input.GetMouseButton(0))
		{
			Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

            RaycastHit raycastInfo;
            
            // unit move target
            if (Physics.Raycast(ray, out raycastInfo, Mathf.Infinity, floorLayer))
		    {
				Vector3 newPos = raycastInfo.point;
                Vector3 targetPos = newPos;
                targetPos.y += 0.1f;
                targetCursor.transform.position = targetPos;

                playerCar.SetTargetPos(newPos);
			}
		}
    }
}
