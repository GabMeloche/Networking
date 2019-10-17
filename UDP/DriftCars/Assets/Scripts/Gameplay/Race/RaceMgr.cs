using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RaceMgr : MonoBehaviour
{
    [SerializeField]
    List<GameObject> PlayerPrefabList;
    [SerializeField]
    List<Transform> PlayerStartList;
    [SerializeField]
    Text RaceMsgText = null;
    [SerializeField]
    Text LapCountText = null;
    [SerializeField, Range(1, 3)]
    int NbLaps = 3;
    int crtLap = 1;
    [SerializeField, Range(1, 4)]
    int NbPlayers = 1;

    // validated checkpoint per lap for each car
    List<List<bool>> validatedCheckpoints;

    SimpleCamera cam = null;

    List<GameObject> playerCarGaoList;

    // car controlled by local player
    Car playerCar;
    public Car PlayerCar { get { return playerCar; } }

    enum ERaceState
    {
        WAITING,
        STARTED,
        FINISHED
    }

    ERaceState raceState = ERaceState.WAITING;
    public bool HasStarted { get { return raceState == ERaceState.STARTED; } }

    public delegate void RaceEventDelegate();
    public event RaceEventDelegate OnRaceInitialized;
    public event RaceEventDelegate OnRaceFinished;

    void Start()
    {
        GameMgr.Instance.OnStartRace += StartRaceSequence;
        GameMgr.Instance.OnResetRace += RestartRace;
        LapCountText.enabled = false;
    }

    void InitRace()
    {
        playerCarGaoList = new List<GameObject>();

        // instantiate wanted number of cars
        for (int i = 0; i < NbPlayers; i++)
        {
            Transform playerStart = PlayerStartList[i];
            GameObject playerCarGao = Instantiate(PlayerPrefabList[i], playerStart.position, playerStart.rotation);
            playerCarGaoList.Add(playerCarGao);
        }
        // follow and control first player
        playerCar = playerCarGaoList[0].GetComponent<Car>();
        cam = Camera.main.GetComponent<SimpleCamera>();
        cam.TargetTr = playerCarGaoList[0].transform;

        // ini checkpoints
        CheckPoint[] checkPoints = FindObjectsOfType<CheckPoint>();
        // register to cp validation event
        foreach (CheckPoint cp in checkPoints)
            cp.OnValidateCheckpoint += ValidateCheckpoint;

        validatedCheckpoints = new List<List<bool>>();

        for (int i = 0; i < NbPlayers; i++)
        {
            validatedCheckpoints.Add(new List<bool>());
            foreach (CheckPoint cp in checkPoints)
            {
                validatedCheckpoints[i].Add(false);
            }
        }

        // init lap GUI
        LapCountText.enabled = true;
        UpdateLapUI();

        OnRaceInitialized();
    }

    void StartRaceSequence()
    {
        InitRace();

        StartCoroutine(StartRace_Coroutine());
        //StartRace();
    }

    IEnumerator StartRace_Coroutine()
    {
        RaceMsgText.text = "3";
        yield return new WaitForSeconds(1f);
        RaceMsgText.text = "2";
        yield return new WaitForSeconds(1f);
        RaceMsgText.text = "1";
        yield return new WaitForSeconds(1f);
        RaceMsgText.text = "GO !!";
        StartRace();
        yield return new WaitForSeconds(1f);
        RaceMsgText.enabled = false;
    }

    void StartRace()
    {
        raceState = ERaceState.STARTED;
    }

    void RestartRace()
    {
        // clean up race objects
        cam.TargetTr = null;
        // reset checkpoints
        validatedCheckpoints.Clear();
        // reset players
        for (int i = 0; i < playerCarGaoList.Count; i++)
            Destroy(playerCarGaoList[i]);
        playerCarGaoList.Clear();

        StartRaceSequence();
    }

    void UpdateLapUI()
    {
        LapCountText.text = "Lap " + crtLap.ToString() + " / " + NbLaps.ToString();
    }

    void ValidateCheckpoint(CheckPoint cp, Car car)
    {
        if (raceState == ERaceState.FINISHED)
            return;

        int carId = car.ID;
        if (validatedCheckpoints.Count >= carId)
        {
            if (cp.ID == 0)
            {
                validatedCheckpoints[carId][cp.ID] = true;
                //Debug.Log("cp validated " + cp.ID);
            }
            else if (validatedCheckpoints[carId].Count >= cp.ID)
            {
                if (validatedCheckpoints[carId][cp.ID - 1])
                {
                    validatedCheckpoints[carId][cp.ID] = true;
                    //Debug.Log("cp validated " + cp.ID);
                }
            }

            if (validatedCheckpoints[carId].Count - 1 == cp.ID)
            {
                foreach (bool b in validatedCheckpoints[carId])
                {
                    if (b == false)
                        return;
                }
                if (crtLap < NbLaps)
                {
                    crtLap++;
                    UpdateLapUI();

                    // reset checkpoints
                    for (int i = 0; i < validatedCheckpoints[carId].Count; i++)
                    {
                        validatedCheckpoints[carId][i] = false;
                    }
                }
                else
                {
                    Debug.Log("race complete");
                    RaceMsgText.text = "Race Complete ! Winner is car " + carId;
                    RaceMsgText.enabled = true;
                    raceState = ERaceState.FINISHED;
                    OnRaceFinished();
                }
            }
        }
    }
}
