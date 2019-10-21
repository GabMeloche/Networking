using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Runtime.InteropServices;

public class GameMgr : MonoBehaviour
{
    static GameMgr instance = null;
    static public GameMgr Instance { get { return instance; } }

    static public RaceMgr Race { get { return instance.GetRaceMgr; } }

    RaceMgr raceMgr;
    public RaceMgr GetRaceMgr { get { return raceMgr; } }

    public delegate void GameEventDelegate();

    public event GameEventDelegate OnServerClick;
    public event GameEventDelegate OnClientClick;
    public event GameEventDelegate OnStartRace;
    public event GameEventDelegate OnResetRace;

    [SerializeField]
    Button startBt;
    [SerializeField]
    Button resetBt;
    [SerializeField]
    Button clientBt;
    [SerializeField]
    Button serverBt;

    void Awake()
    {
        instance = this;
        raceMgr = GetComponentInChildren<RaceMgr>();


        clientBt.gameObject.SetActive(true);
        serverBt.gameObject.SetActive(true);

        startBt.gameObject.SetActive(false);
        resetBt.gameObject.SetActive(false);
    }

    void Start()
    {
        // register events

        raceMgr.OnRaceFinished += () =>
        {
            resetBt.gameObject.SetActive(true);
        };

        serverBt.onClick.AddListener(
           () =>
            {
                serverBt.gameObject.SetActive(false);
                clientBt.gameObject.SetActive(false);
                startBt.gameObject.SetActive(true);
                gameObject.AddComponent<UDP_server>();
                //launch server
            }
            );

        clientBt.onClick.AddListener(
            () =>
            {
                clientBt.gameObject.SetActive(false);
                serverBt.gameObject.SetActive(false);
                startBt.gameObject.SetActive(true);
                //launch client
            }
        );

        startBt.onClick.AddListener(
        () =>
        {
            OnStartRace();
            startBt.gameObject.SetActive(false);
        }
        );

        resetBt.onClick.AddListener(
        () =>
        {
            OnResetRace();
            resetBt.gameObject.SetActive(false);
        }
        );
    }
}
